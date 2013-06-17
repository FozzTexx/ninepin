/* Copyright 2013 by Chris Osborn <fozztexx@fozztexx.com>
 *
 * $Id$
 */

/* FIXME - put some easter eggs in here referencing the R.E.M. song
   "Driver 8" because it's a driver to emulate the 1541 which is
   normally device number 8. */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#define DRIVER_AUTHOR	"Chris Osborn <fozztexx@fozztexx.com>"
#define DRIVER_DESC	"Commodore IEC serial driver"

#define IEC_BUFSIZE	1024

#define IEC_ATN		25
#define IEC_CLKIN	8
#define IEC_DATAIN	7
#define IEC_CLKOUT	10
#define IEC_DATAOUT	9

#define LABEL_ATN	"IEC attention pin"
#define LABEL_CLK	"IEC clock pin"
#define LABEL_DATA	"IEC data pin"
#define LABEL_DEVICE	"cbm-iec driver"
#define LABEL_WORK	"IEC work"

#define DATA_EOI	0x100
#define DATA_ATN	0x200

enum {
  IECWaitState = 1,
  IECAttentionState,
  IECListenState,
  IECTalkState,
  IECCanSendState,
  IECDoSendState,
};

#define BCM2708_PERI_BASE   0x20000000
#define GPIO_BASE  (BCM2708_PERI_BASE + 0x200000)

#define INPUT	0
#define OUTPUT	1
#define LOW	0
#define HIGH	1

#define digitalRead(pin)	({int _p = (pin) & 31; (*(gpio + 13) & (1 << _p)) >> _p;})
#define digitalWrite(pin, val)	({int _p = (pin) & 31; *(gpio + 7 + ((val) ? 0 : 3)) = \
							 1 << _p;})
#define pinMode(pin, mode)	({int _p = (pin) & 31; *(gpio + _p / 10) = \
							 (*(gpio + _p / 10) & \
							  ~(7 << (_p % 10) * 3)) | \
							 ((mode) << (_p % 10) * 3);})

struct iec_data {
  unsigned int command:3;
  unsigned int device:5;
  unsigned int secondary:4;
  unsigned int channel:4;
  uint16_t len;
  void *data;
  int pos;
};

static short int irq_atn = 0, irq_clk = 0;
static int iec_major = 60;
static uint16_t *iec_buffer;
static short iec_inpos, iec_outpos;
static volatile uint32_t *gpio;
static int iec_state = 0;
static struct workqueue_struct *iec_wq;
static int c64slowdown = 10;

static irqreturn_t iec_handleCLK(int irq, void *dev_id, struct pt_regs *regs);
static irqreturn_t iec_handleATN(int irq, void *dev_id, struct pt_regs *regs);
static void iec_sendData(struct work_struct *work);
int iec_open(struct inode *inode, struct file *filp);
int iec_close(struct inode *inode, struct file *filp);
ssize_t iec_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

DECLARE_WORK(iec_work, iec_sendData);

struct file_operations iec_fops = {
 owner: THIS_MODULE,
 read: iec_read,
 write: iec_write,
 open: iec_open,
 release: iec_close
};

static inline int iec_waitForSignal(int pin, int val, int delay)
{
  struct timeval start, now;
  int elapsed, abort = 0;


  do_gettimeofday(&start);
  while (digitalRead(pin) != val) {
    do_gettimeofday(&now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
    if (elapsed >= delay) {
      abort = 1;
      break;
    }
  }

  return abort;
}

int iec_readByte(void)
{
  int eoi, abort;
  int len, bits;
  struct timeval start, now;
  int elapsed = 0;

#if 0
  pinMode(IEC_DATA, INPUT);
#else
  digitalWrite(IEC_DATAOUT, HIGH);
#endif

  do_gettimeofday(&start);
  for (abort = eoi = 0; !abort && digitalRead(IEC_CLKIN); ) {
    do_gettimeofday(&now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);

    if (!eoi && elapsed >= 200) {
#if 0
      pinMode(IEC_DATA, OUTPUT);
      udelay(c64slowdown*2);
      pinMode(IEC_DATA, INPUT);
#else
      digitalWrite(IEC_DATAOUT, LOW);
      udelay(c64slowdown*2);
      digitalWrite(IEC_DATAOUT, HIGH);
#endif
      eoi = DATA_EOI;
    }

    if (elapsed > 10000) {
      printk(KERN_NOTICE "IEC: Timeout during start\n");
      abort = 1;
      break;
    }
  }

  if (elapsed < 60) {
    len = elapsed - c64slowdown;
    if (len > 0 && c64slowdown / 10 < len && len > 5) {
      c64slowdown = elapsed;
      printk(KERN_NOTICE "IEC: calibrating delay: %i\n", elapsed);
    }
  }

  for (len = 0, bits = eoi; !abort && len < 8; len++) {
    if ((abort = iec_waitForSignal(IEC_CLKIN, 1, 150))) {
      printk(KERN_NOTICE "IEC: timeout waiting for bit %i\n", len);
      break;
    }

    if (digitalRead(IEC_DATAIN))
      bits |= 1 << len;

    if (iec_waitForSignal(IEC_CLKIN, 0, 150)) {
      printk(KERN_NOTICE "IEC: Timeout after bit %i\n", len);
      if (len < 7)
	abort = 1;
    }
  }

#if 0
  pinMode(IEC_DATA, OUTPUT);
#else
  digitalWrite(IEC_DATAOUT, LOW);
#endif
  udelay(c64slowdown);

  if (abort)
    return -1;

  return bits;
}
  
void iec_releaseBus(void)
{
#if 0
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, INPUT);
#else
  digitalWrite(IEC_CLKOUT, HIGH);
  digitalWrite(IEC_DATAOUT, HIGH);
#endif
  udelay(c64slowdown);
  return;
}

int iec_setupTalker(void)
{
  int abort = 0;


  abort = iec_waitForSignal(IEC_ATN, 1, 1000000);  
  if (!abort && (abort = iec_waitForSignal(IEC_CLKIN, 1, 100000)))
    printk(KERN_NOTICE "IEC: Timeout waiting for start of talk\n");

  if (!abort) {
#if 0
    pinMode(IEC_DATA, INPUT);
    pinMode(IEC_CLK, OUTPUT);
#else
    digitalWrite(IEC_DATAOUT, HIGH);
    digitalWrite(IEC_CLKOUT, LOW);
#endif
    udelay(c64slowdown);
    iec_state = IECDoSendState;
  }

  return abort;
}

static irqreturn_t iec_handleCLK(int irq, void *dev_id, struct pt_regs *regs)
{
  unsigned long flags;
  int atn, val, cmd, dev;
  int abort;


  //printk(KERN_NOTICE "IEC: clock %i\n", iec_state);
  if (iec_state == IECWaitState || iec_state == IECDoSendState)
    return IRQ_HANDLED;

  if (iec_state == IECCanSendState) {
    //if (!iec_setupTalker())
    queue_work(iec_wq, &iec_work);
    return IRQ_HANDLED;
  }
  
  // disable hard interrupts (remember them in flag 'flags')
  local_irq_save(flags);
  
  abort = 0;
  atn = !digitalRead(IEC_ATN);

  val = iec_readByte();
  //printk(KERN_NOTICE "IEC: Read: %03x\n", val);
  if (val >= 0) {
    iec_buffer[iec_inpos] = val;
    iec_inpos = (iec_inpos + 1) % IEC_BUFSIZE;

    if (val & DATA_EOI)
      iec_state = IECWaitState;
    else if (atn) {
      cmd = val & 0xe0;
      dev = val & 0x1f;

      switch (cmd) {
      case 0x20: /* Listen */
	if (dev != 8) {
	  /* FIXME - ignore everything until ATN goes high */
	  if (dev == 31)
	    iec_state = IECWaitState;
	  udelay(c64slowdown);
	  iec_releaseBus();
	}
	else
	  iec_state = IECListenState;
	break;

      case 0x40: /* Talk */
	if (dev != 8) {
	  /* FIXME - ignore everything until ATN goes high */
	  if (dev == 31)
	    iec_state = IECWaitState;
	  udelay(c64slowdown);
	  iec_releaseBus();
	}
	else
	  iec_state = IECTalkState;
	break;

      case 0x60: /* Channel */
	if (iec_state == IECTalkState) {
	  iec_state = IECCanSendState;
#if 0
	  if (!iec_setupTalker())
	    queue_work(iec_wq, &iec_work);
#endif
	}
	break;

      case 0xE0: /* Open/Close */
	break;
      }
    }
  }
  
  // restore hard interrupts
  local_irq_restore(flags);

  return IRQ_HANDLED;
}

static irqreturn_t iec_handleATN(int irq, void *dev_id, struct pt_regs *regs)
{
  unsigned long flags;


  // disable hard interrupts (remember them in flag 'flags')
  local_irq_save(flags);

  //printk(KERN_NOTICE "IEC: attention\n");
  iec_state = IECAttentionState;
#if 0
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, OUTPUT);
#else
  digitalWrite(IEC_CLKOUT, HIGH);
  digitalWrite(IEC_DATAOUT, LOW);
#endif
  udelay(c64slowdown);
  
  // restore hard interrupts
  local_irq_restore(flags);

  return IRQ_HANDLED;
}

int iec_writeByte(int bits)
{
  int len;
  int abort = 0;
  unsigned long flags;


  disable_irq(irq_clk);
  local_irq_save(flags);
  //printk(KERN_NOTICE "IEC: Write: %03x data: %i\n", bits, digitalRead(IEC_DATA));
#if 0
  pinMode(IEC_CLK, INPUT);
#else
  digitalWrite(IEC_CLKOUT, HIGH);
#endif

  if ((abort = iec_waitForSignal(IEC_DATAIN, 1, 100000)))
    printk(KERN_NOTICE "IEC: Timeout waiting to send\n");

  if (!abort && (bits & DATA_EOI)) {
    //printk(KERN_NOTICE "IEC: sending EOI\n");
    if ((abort = iec_waitForSignal(IEC_DATAIN, 0, 100000)))
      printk(KERN_NOTICE "IEC: Timeout waiting for EOI ack\n");

    if (!abort && (abort = iec_waitForSignal(IEC_DATAIN, 1, 100000)))
      printk(KERN_NOTICE "IEC: Timeout waiting for EOI ack finish\n");
  }

#if 0
  pinMode(IEC_CLK, OUTPUT);
#else
  digitalWrite(IEC_CLKOUT, LOW);
#endif
  local_irq_restore(flags);
  udelay(c64slowdown);
  
  for (len = 0; !abort && len < 8; len++, bits >>= 1) {
    if (iec_state != IECDoSendState) {
      abort = 1;
      break;
    }
#if 0
    if (bits & 1)
      pinMode(IEC_DATA, INPUT);
    else
      pinMode(IEC_DATA, OUTPUT);
#else
    digitalWrite(IEC_DATAOUT, bits & 1);
#endif
    udelay(c64slowdown*2);
#if 0
    pinMode(IEC_CLK, INPUT);
#else
    digitalWrite(IEC_CLKOUT, HIGH);
#endif
    udelay(c64slowdown*2);
#if 0
    pinMode(IEC_DATA, INPUT);
    pinMode(IEC_CLK, OUTPUT);
#else
    digitalWrite(IEC_DATAOUT, HIGH);
    digitalWrite(IEC_CLKOUT, LOW);
#endif
  }

  if (!abort && (abort = iec_waitForSignal(IEC_DATAIN, 0, 10000))) {
    printk(KERN_NOTICE "IEC: Timeout waiting for listener ack\n");
    abort = 0;
  }

  enable_irq(irq_clk);

  udelay(c64slowdown);
  
  return abort;
}

static void iec_sendData(struct work_struct *work)
{
  int abort;
  int bpos, blen;
  int val;
  static unsigned char buf[] = {
    0x01, 0x04, 0x01, 0x01, 0x00, 0x00, 0x12, 0x22,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x20, 0x30, 0x30, 0x20, 0x32, 0x41, 0x00,

    0x01, 0x01, 0x75, 0x00, 0x20, 0x22, 0x41, 0x52,
    0x4D, 0x59, 0x20, 0x4D, 0x4F, 0x56, 0x45, 0x53,
    0x20, 0x31, 0x22, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x50, 0x52, 0x47, 0x20, 0x20, 0x20, 0x20, 0x00,
    
    0x01, 0x01, 0x87, 0x00, 0x20, 0x22, 0x41, 0x52,
    0x4D, 0x59, 0x20, 0x4D, 0x4F, 0x56, 0x45, 0x53,
    0x20, 0x32, 0x22, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x50, 0x52, 0x47, 0x20, 0x20, 0x20, 0x20, 0x00,

    0x01, 0x01, 0x29, 0x00, 0x20, 0x20, 0x22, 0x41,
    0x52, 0x4D, 0x59, 0x20, 0x4D, 0x4F, 0x56, 0x45,
    0x53, 0x20, 0x50, 0x49, 0x43, 0x22, 0x20, 0x20,
    0x20, 0x50, 0x52, 0x47, 0x20, 0x20, 0x20, 0x00,

    0x01, 0x01, 0x01, 0x00, 0x20, 0x20, 0x20, 0x22,
    0x45, 0x52, 0x52, 0x4F, 0x52, 0x43, 0x48, 0x41,
    0x4E, 0x4E, 0x45, 0x4C, 0x22, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x50, 0x52, 0x47, 0x20, 0x20, 0x00,

    0x01, 0x01, 0x72, 0x01, 0x42, 0x4C, 0x4F, 0x43,
    0x4B, 0x53, 0x20, 0x46, 0x52, 0x45, 0x45, 0x2E,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00
  };


  abort = 0;
  if (iec_state == IECCanSendState)
    abort = iec_setupTalker();
  
  for (bpos = 0, blen = sizeof(buf);
       !abort && iec_state == IECDoSendState && bpos < blen;
       bpos++) {
    //printk(KERN_NOTICE "IEC: sending %i of %i\n", bpos, blen);
    val = buf[bpos];
    if (bpos == blen - 1)
      val |= DATA_EOI;
    abort = iec_writeByte(val);
  }

  if (abort) {
    iec_state = IECWaitState;
    iec_releaseBus();
  }
  
  return;
}

int iec_init(void)
{
  int result;
  struct resource *mem;


  /* FIXME - if allocation fails for anything clean up what didn't fail */
  
  /* FIXME - dynamically allocate entry in dev with dynamic major */
  /* http://www.makelinux.com/ldd3/ */
  /* http://stackoverflow.com/questions/5970595/create-a-device-node-in-code */
  
  mem = request_mem_region(GPIO_BASE, 4096, LABEL_DEVICE);
  gpio = ioremap(GPIO_BASE, 4096);

  pinMode(IEC_ATN, INPUT);
  pinMode(IEC_CLKIN, INPUT);
  pinMode(IEC_DATAIN, INPUT);

#if 0
  digitalWrite(IEC_CLK, LOW);
  digitalWrite(IEC_DATA, LOW);
#else
  pinMode(IEC_CLKOUT, OUTPUT);
  pinMode(IEC_DATAOUT, OUTPUT);

  digitalWrite(IEC_CLKOUT, HIGH);
  digitalWrite(IEC_DATAOUT, HIGH);
#endif
  
  if ((result = register_chrdev(iec_major, LABEL_DEVICE, &iec_fops)) < 0) {
    printk(KERN_NOTICE "IEC: cannot obtain major number %i\n", iec_major);
    goto fail_chrdev;
  }

  if (!(iec_buffer = kmalloc(IEC_BUFSIZE * sizeof(uint16_t), GFP_KERNEL))) {
    printk(KERN_NOTICE "IEC: failed to allocate buffer\n");
    result = -ENOMEM;
    goto fail_buffer;
  }
  iec_inpos = iec_outpos = 0;

  if ((result = gpio_request(IEC_ATN, LABEL_ATN))) {
    printk(KERN_NOTICE "IEC: GPIO request faiure: %s\n", LABEL_ATN);
    goto fail_atn;
  }
  if ((result = gpio_request(IEC_CLKIN, LABEL_CLK))) {
    printk(KERN_NOTICE "IEC: GPIO request faiure: %s\n", LABEL_CLK);
    goto fail_clk;
  }
  if ((result = gpio_request(IEC_DATAIN, LABEL_DATA))) {
    printk(KERN_NOTICE "IEC: GPIO request faiure: %s\n", LABEL_DATA);
    goto fail_data;
  }

  if ((irq_atn = gpio_to_irq(IEC_ATN)) < 0) {
    printk(KERN_NOTICE "IEC: GPIO to IRQ mapping faiure %s\n", LABEL_ATN);
    result = irq_atn;
    goto fail_mapatn;
  }

  if ((irq_clk = gpio_to_irq(IEC_CLKIN)) < 0) {
    printk(KERN_NOTICE "IEC: GPIO to IRQ mapping faiure %s\n", LABEL_CLK);
    result = irq_clk;
    goto fail_mapclk;
  }

  /* FIXME - don't enable interrupts until user space opens driver */
  if ((result = request_irq(irq_atn, (irq_handler_t) iec_handleATN, 
			 IRQF_TRIGGER_FALLING, LABEL_ATN, LABEL_DEVICE))) {
    printk(KERN_NOTICE "IEC: IRQ Request failure\n");
    goto fail_irqatn;
  }

  if ((result = request_irq(irq_clk, (irq_handler_t) iec_handleCLK, 
			    IRQF_TRIGGER_RISING, LABEL_CLK, LABEL_DEVICE))) {
    printk(KERN_NOTICE "IEC: IRQ Request failure\n");
    goto fail_irqclk;
  }

  //disable_irq(irq_clk);
  
  printk(KERN_NOTICE "IEC module loaded\n");

  iec_wq = create_singlethread_workqueue(LABEL_WORK);
  
  return 0;

 fail_irqclk:
  free_irq(irq_atn, LABEL_DEVICE);
 fail_irqatn:
 fail_mapclk:
 fail_mapatn:
  gpio_free(IEC_DATAIN);
 fail_data:
  gpio_free(IEC_CLKIN);
 fail_clk:
  gpio_free(IEC_ATN);
 fail_atn:
  kfree(iec_buffer);
 fail_buffer:
  unregister_chrdev(iec_major, LABEL_DEVICE);
 fail_chrdev:
  return result;
}

void iec_cleanup(void)
{
  destroy_workqueue(iec_wq);
  unregister_chrdev(iec_major, LABEL_DEVICE);
  kfree(iec_buffer);

  free_irq(irq_atn, LABEL_DEVICE);
  free_irq(irq_clk, LABEL_DEVICE);
  gpio_free(IEC_ATN);
  gpio_free(IEC_CLKIN);
  gpio_free(IEC_DATAIN);

  iounmap(gpio);
  printk(KERN_NOTICE "IEC module removed\n");
  return;
}

int iec_open(struct inode *inode, struct file *filp)
{
  return 0;
}

int iec_close(struct inode *inode, struct file *filp)
{
  return 0;
}

ssize_t iec_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  unsigned long remaining;
  int avail;


  avail = (IEC_BUFSIZE + iec_inpos - iec_outpos) % IEC_BUFSIZE;
  avail *= 2;
  /* FIXME - if avail is zero, block */
  
  if (count % 2)
    count--;
  if (count > avail)
    count = avail;
  remaining = copy_to_user(buf, &iec_buffer[iec_outpos], count);
  iec_outpos += (count - remaining) / 2;

  if (*f_pos == 0) {
    *f_pos += count;
    return count;
  }

  return 0;
}

ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  return 1;
}

module_init(iec_init);
module_exit(iec_cleanup);

/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
