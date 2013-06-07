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
#define IEC_CLK		8
#define IEC_DATA	7

#define LABEL_ATN	"IEC attention pin"
#define LABEL_CLK	"IEC clock pin"
#define LABEL_DATA	"IEC data pin"
#define LABEL_DEVICE	"cbm-iec driver"

#define DATA_EOI	0x100
#define DATA_ATN	0x200

enum {
  IECWaitState = 1,
  IECListenState,
  IECTalkState,
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

static short int irq_atn = 0, irq_clk = 0;
static int clk_enabled = 0;
static int iec_major = 60;
static uint16_t *iec_buffer;
static short iec_inpos, iec_outpos;
static volatile uint32_t *gpio;
static int iec_state = 0;

static irqreturn_t iec_handleCLK(int irq, void *dev_id, struct pt_regs *regs);
static irqreturn_t iec_handleATN(int irq, void *dev_id, struct pt_regs *regs);
int iec_open(struct inode *inode, struct file *filp);
int iec_close(struct inode *inode, struct file *filp);
ssize_t iec_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

struct file_operations iec_fops = {
 owner: THIS_MODULE,
 read: iec_read,
 write: iec_write,
 open: iec_open,
 release: iec_close
};

int iec_readByte(void)
{
  int eoi, abort, elapsed;
  int len, bits;
  struct timeval start, now;

  
  pinMode(IEC_DATA, INPUT);

  abort = 0;
#if 0
  do_gettimeofday(&start);
  while (!digitalRead(IEC_CLK)) {
    do_gettimeofday(&now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
    if (elapsed >= 100000) {
      printk(KERN_NOTICE "IEC: Timeout waiting for talker\n");
      abort = 1;
      break;
    }
  }
#endif

  do_gettimeofday(&start);
  for (eoi = 0; !abort && digitalRead(IEC_CLK); ) {
    do_gettimeofday(&now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);

    if (!eoi && elapsed >= 200) {
      pinMode(IEC_DATA, OUTPUT);
      udelay(80);
      pinMode(IEC_DATA, INPUT);
      eoi = 1;
    }

    if (elapsed > 10000) {
      printk(KERN_NOTICE "IEC: Timeout during start\n");
      abort = 1;
      break;
    }
  }

  for (len = bits = 0; !abort && len < 8; len++) {
    do_gettimeofday(&start);
    while (!digitalRead(IEC_CLK)) {
      do_gettimeofday(&now);
      elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
      if (elapsed >= 10000) {
	printk(KERN_NOTICE "IEC: timeout waiting for bit %i\n", len);
	abort = 1;
	break;
      }
    }

    if (abort)
      break;

    if (digitalRead(IEC_DATA))
      bits |= 1 << len;

    do_gettimeofday(&start);
    while (digitalRead(IEC_CLK)) {
      do_gettimeofday(&now);
      elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
      if (elapsed >= 10000) {
	printk(KERN_NOTICE "IEC: Timeout after bit %i %i\n", len, elapsed);
	if (len < 7)
	  abort = 1;
	break;
      }
    }
  }

  pinMode(IEC_DATA, OUTPUT);

  if (abort)
    return -1;

  if (eoi)
    bits |= DATA_EOI;
  return bits;
}
  
int iec_waitForATNEnd(void)
{
  struct timeval start, now;
  int elapsed;
  int abort = 0;

  
  do_gettimeofday(&start);
  while (!digitalRead(IEC_ATN)) {
    do_gettimeofday(&now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
    if (elapsed >= 100000) {
      printk(KERN_NOTICE "IEC: Timeout waiting for end of ATN\n");
      abort = 1;
      break;
    }
  }

  return abort;
}

void iec_enableClockIRQ(int flag)
{
  if (flag && !clk_enabled) {
    //enable_irq(irq_clk);
    clk_enabled++;
  }
  else if (!flag && clk_enabled) {
    //disable_irq(irq_clk);
    clk_enabled--;
  }

  return;
}

int iec_releaseBus(void)
{
  int abort = 0;

  
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, INPUT);
  //abort = iec_waitForATNEnd();
  iec_enableClockIRQ(0);
  iec_state = IECWaitState;
  return abort;
}

static irqreturn_t iec_handleCLK(int irq, void *dev_id, struct pt_regs *regs)
{
  unsigned long flags;
  int atn, val, cmd, dev;
  int abort;


  atn = !digitalRead(IEC_ATN);
  if (!atn && iec_state != IECListenState)
    return IRQ_HANDLED;
  
  // disable hard interrupts (remember them in flag 'flags')
  local_irq_save(flags);

  printk(KERN_NOTICE "IEC: clock\n");
  
  abort = 0;

  val = iec_readByte();
  printk(KERN_NOTICE "IEC: Read: %03x\n", val);
  if (val >= 0) {
    iec_buffer[iec_inpos] = val;
    iec_inpos = (iec_inpos + 1) % IEC_BUFSIZE;

    if (val & DATA_EOI)
      iec_releaseBus();
    else if (atn) {
      cmd = val & 0xe0;
      dev = val & 0x1f;

      switch (cmd) {
      case 0x20: /* Listen */
	if (dev != 8)
	  iec_releaseBus();
	else {
	  iec_state = IECListenState;
	  iec_enableClockIRQ(1);
	}
	break;

      case 0x40: /* Talk */
	if (dev != 8)
	  iec_releaseBus();
	else {
	  iec_state = IECTalkState;
	  iec_enableClockIRQ(1);
	}
	break;

      case 0x60: /* Channel */
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
  int atn;


  // disable hard interrupts (remember them in flag 'flags')
  local_irq_save(flags);

  printk(KERN_NOTICE "IEC: attention\n");
  
  atn = !digitalRead(IEC_ATN);
  if (atn) {
    iec_enableClockIRQ(1);
    pinMode(IEC_CLK, INPUT);
    pinMode(IEC_DATA, OUTPUT);
  }
  
  // restore hard interrupts
  local_irq_restore(flags);

  return IRQ_HANDLED;
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
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, INPUT);

  digitalWrite(IEC_CLK, LOW);
  digitalWrite(IEC_DATA, LOW);

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
  if ((result = gpio_request(IEC_CLK, LABEL_CLK))) {
    printk(KERN_NOTICE "IEC: GPIO request faiure: %s\n", LABEL_CLK);
    goto fail_clk;
  }
  if ((result = gpio_request(IEC_DATA, LABEL_DATA))) {
    printk(KERN_NOTICE "IEC: GPIO request faiure: %s\n", LABEL_DATA);
    goto fail_data;
  }

  if ((irq_atn = gpio_to_irq(IEC_ATN)) < 0) {
    printk(KERN_NOTICE "IEC: GPIO to IRQ mapping faiure %s\n", LABEL_ATN);
    result = irq_atn;
    goto fail_mapatn;
  }

  if ((irq_clk = gpio_to_irq(IEC_CLK)) < 0) {
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

  return 0;

 fail_irqclk:
  free_irq(irq_atn, LABEL_DEVICE);
 fail_irqatn:
 fail_mapclk:
 fail_mapatn:
  gpio_free(IEC_DATA);
 fail_data:
  gpio_free(IEC_CLK);
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
  unregister_chrdev(iec_major, LABEL_DEVICE);
  kfree(iec_buffer);

  free_irq(irq_atn, LABEL_DEVICE);
  free_irq(irq_clk, LABEL_DEVICE);
  gpio_free(IEC_ATN);
  gpio_free(IEC_CLK);
  gpio_free(IEC_DATA);

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
