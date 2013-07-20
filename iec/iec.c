/* Copyright 2013 by Chris Osborn <fozztexx@fozztexx.com>
 *
 * This file is part of ninepin.
 *
 * ninepin is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * ninepin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ninepin; see the file COPYING. If not see
 * <http://www.gnu.org/licenses/>.
 */

#include "iec.h"
#include "gpio.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/poll.h>

#define DRIVER_AUTHOR	"Chris Osborn <fozztexx@fozztexx.com>"
#define DRIVER_DESC	"Commodore IEC serial driver"

#define IEC_BUFSIZE	1024
#define IEC_FIRSTDEV	4
#define IEC_ALLDEV	31

/* FIXME - make number of pins be a driver option when loading */
#define NUMPINS		3
#define IEC_ATN		25
#if NUMPINS == 3
#define IEC_CLK		8
#define IEC_DATA	7
#else
#define IEC_CLKIN	8
#define IEC_DATAIN	7
#define IEC_CLKOUT	10
#define IEC_DATAOUT	9
#endif

#define LABEL_ATN	"IEC attention pin"
#define LABEL_CLK	"IEC clock pin"
#define LABEL_DATA	"IEC data pin"
#define LABEL_DEVICE	"iec"
#define LABEL_WRITE	"IEC write queue"
#define LABEL_READ	"IEC read queue"

#define DATA_EOI	0x100
#define DATA_ATN	0x200

enum {
  IECWaitState = 1,
  IECAttentionState,
  IECAttentionIgnoreState,
  IECListenState,
  IECInputState,
  IECTalkState,
  IECChannelState,
  IECOutputState,
};

typedef struct {
  unsigned char data[IEC_BUFSIZE];
  void *next;
} iec_linkedData;

typedef struct {
  iec_data header;
  iec_linkedData *data;
  void *next;
  int pos;
} iec_io;

typedef struct {
  iec_io *head, *tail, *cur;
} iec_chain;

typedef struct {
  iec_chain in, out;
  int flags;
  struct semaphore lock;
} iec_device;

static irqreturn_t iec_handleCLK(int irq, void *dev_id, struct pt_regs *regs);
static irqreturn_t iec_handleATN(int irq, void *dev_id, struct pt_regs *regs);
static void iec_processData(struct work_struct *work);
static void iec_sendData(struct work_struct *work);
int iec_open(struct inode *inode, struct file *filp);
int iec_close(struct inode *inode, struct file *filp);
unsigned int iec_poll(struct file *filp, poll_table *wait);
ssize_t iec_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

static iec_device *iec_openDevices[IEC_ALLDEV];
static int iec_deviceCount = 0, iec_curDevice, iec_curChannel;
static int iec_readAvail = 0;
static short int irq_atn = 0, irq_clk = 0;
static dev_t iec_devno;
static struct cdev iec_cdev;
static struct class *iec_class;
static uint16_t *iec_buffer;
static short iec_inpos, iec_outpos;
static int iec_state = 0, iec_atnState = 0;
static struct workqueue_struct *iec_writeQ, *iec_readQ;
static int c64slowdown = 10;
DECLARE_WORK(iec_writeWork, iec_sendData);
DECLARE_WORK(iec_readWork, iec_processData);
static DECLARE_WAIT_QUEUE_HEAD(iec_wait);

static volatile uint32_t *gpio;

struct file_operations iec_fops = {
 owner: THIS_MODULE,
 read: iec_read,
 write: iec_write,
 poll: iec_poll,
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

void iec_releaseBus(void)
{
#if NUMPINS == 3
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, INPUT);
#else
  digitalWrite(IEC_CLKOUT, HIGH);
  digitalWrite(IEC_DATAOUT, HIGH);
#endif
  udelay(c64slowdown);
  return;
}

static irqreturn_t iec_handleATN(int irq, void *dev_id, struct pt_regs *regs)
{
  int atn;

  
  atn = !digitalRead(IEC_ATN);
  //printk(KERN_NOTICE "IEC: attention %i\n", atn);
  if (atn) {
    iec_atnState = IECAttentionState;
    iec_state = IECWaitState;
#if NUMPINS == 3
    pinMode(IEC_CLK, INPUT);
    pinMode(IEC_DATA, OUTPUT);
#else
    digitalWrite(IEC_CLKOUT, HIGH);
    digitalWrite(IEC_DATAOUT, LOW);
#endif
  }
  else
    iec_atnState = IECWaitState;
  
  return IRQ_HANDLED;
}

void iec_newIO(int val, int inout)
{
  int dev;
  iec_device *device;
  iec_chain *chain;
  iec_io *io;
  iec_linkedData *data;
  

  dev = val & 0x1f;
  device = iec_openDevices[dev];
  if (!device)
    return;

  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  
  /* FIXME - this shouldn't happen */
  if (chain->cur)
    printk(KERN_NOTICE "IEC: new IO with IO still open\n");

  if (down_interruptible(&device->lock)) {
    printk(KERN_NOTICE "IEC: unable to lock IO\n");
    return;
  }

  io = kmalloc(sizeof(iec_io), GFP_KERNEL);
  io->header.command = val & 0xff;
  io->header.channel = 0;
  io->header.len = 0;
  data = kmalloc(sizeof(iec_linkedData), GFP_KERNEL);
  io->data = data;
  data->next = NULL;
  io->next = NULL;
  io->pos = 0;
  
  if (!chain->head)
    chain->head = chain->tail = io;
  else {
    chain->tail->next = io;
    chain->tail = io;
  }
  chain->cur = io;
  up(&device->lock);

  return;
}

void iec_channelIO(int val, int inout)
{
  int cmd, dev;
  iec_device *device;
  iec_chain *chain;


  cmd = val & 0xf0;
  dev = val & 0x0f;
  
  if (!(device = iec_openDevices[iec_curDevice]))
    return;
  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  if (!chain->cur)
    return;

  if (cmd == IECCloseCommand && dev == 0x00) /* Close */
    chain->cur = NULL;
  else
    chain->cur->header.channel = val & 0xff;

  return;
}

static inline void iec_appendData(const char *buf, int buflen, int inout)
{
  iec_device *device;
  iec_chain *chain;
  iec_io *io;
  iec_linkedData *data, *last;
  int pos, count;
  

  /* FIXME - lock device while sending data so we can send incomplete buffers */
  if (!(device = iec_openDevices[iec_curDevice]))
    return;
  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  if (!(io = chain->cur))
    return;

  while (buflen) {
    last = data = io->data;
    pos = io->pos - sizeof(io->header);
    while (data && pos >= IEC_BUFSIZE) {
      last = data;
      data = data->next;
      pos -= IEC_BUFSIZE;
    }

    count = buflen;
    if (count > IEC_BUFSIZE - pos)
      count = IEC_BUFSIZE - pos;
    if (!data) {
      data = last->next = kmalloc(sizeof(iec_linkedData), GFP_KERNEL);
      data->next = NULL;
    }

    memcpy(&data->data[pos], buf, count);
    io->pos += count;
    buflen -= count;
    buf += count;
  }

  return;
}

void iec_appendIO(int val, int inout)
{
  iec_device *device;
  iec_chain *chain;
  iec_io *io;
  iec_linkedData *data, *last;
  int pos;


  if (!(device = iec_openDevices[iec_curDevice]))
    return;
  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  if (!(io = chain->cur))
    return;

  val = val & 0xff;
  data = last = io->data;
  pos = io->header.len;
  while (data->next) {
    last = data;
    data = data->next;
    pos -= IEC_BUFSIZE;
  }

  if (pos == IEC_BUFSIZE) {
    data = last->next = kmalloc(sizeof(iec_linkedData), GFP_KERNEL);
    data->next = NULL;
    pos = 0;
  }
  data->data[pos] = val;
  io->header.len++;

  return;
}

int iec_unlinkIO(iec_device *device, iec_io *io, int inout)
{
  iec_chain *chain;
  iec_io *last;
  iec_linkedData *data, *next;


  //printk(KERN_NOTICE "IEC: unlinking IO\n");
  if (down_interruptible(&device->lock))
    return 0;
    
  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;

  if ((io == chain->head))
    chain->head = io->next;
  else {
    for (last = chain->head; last && io != last->next; last = last->next)
      ;

    if (last)
      last = ((iec_io *) last->next)->next;
  }

  data = io->data;
  while (data) {
    next = data->next;
    kfree(data);
    data = next;
  }
  kfree(io);
  
  up(&device->lock);
  //printk(KERN_NOTICE "IEC: did unlink\n");
  return 1;
}
    
void iec_closeIO(int inout)
{
  iec_device *device;
  iec_chain *chain;
  iec_io *io;


  if (!(device = iec_openDevices[iec_curDevice]))
    return;

  /* FIXME - if we are holding the bus because of this device, release the bus */
  
  if (inout == INPUT)
    chain = &device->in;
  else
    chain = &device->out;
  if (!chain->cur)
    return;

  io = chain->cur;
  chain->cur = NULL;
  if (inout == INPUT) {
    iec_readAvail = 1;
    wake_up_interruptible(&iec_wait);
  }
  else {
    io->pos = sizeof(io->header);
    //printk(KERN_NOTICE "IEC: queueing write %i\n", io->pos);
    queue_work(iec_writeQ, &iec_writeWork);
  }

  return;
}

static inline int iec_readByte(void)
{
  unsigned long flags;
  int eoi, abort;
  int len, bits;
  struct timeval start, now;
  int elapsed = 0;


  local_irq_save(flags);
#if NUMPINS == 3
  pinMode(IEC_DATA, INPUT);
#else
  digitalWrite(IEC_DATAOUT, HIGH);
#endif

  do_gettimeofday(&start);
  for (abort = eoi = 0; !abort && digitalRead(IEC_CLK); ) {
    do_gettimeofday(&now);
    elapsed = (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);

    if (!eoi && elapsed >= 200) {
#if NUMPINS == 3
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

    if (elapsed > 100000) {
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
    if ((abort = iec_waitForSignal(IEC_CLK, 1, 150))) {
      printk(KERN_NOTICE "IEC: timeout waiting for bit %i\n", len);
      break;
    }

    if (digitalRead(IEC_DATA))
      bits |= 1 << len;

    if (iec_waitForSignal(IEC_CLK, 0, 150)) {
      printk(KERN_NOTICE "IEC: Timeout after bit %i\n", len);
      if (len < 7)
	abort = 1;
    }
  }

#if NUMPINS == 3
  pinMode(IEC_DATA, OUTPUT);
#else
  digitalWrite(IEC_DATAOUT, LOW);
#endif
  local_irq_restore(flags);
  
  udelay(c64slowdown);

  if (abort)
    return -1;

  return bits;
}
  
static irqreturn_t iec_handleCLK(int irq, void *dev_id, struct pt_regs *regs)
{
  int atn, val;
  int cmd, dev;
  int abort;


  //printk(KERN_NOTICE "IEC: clock %i %i\n", iec_atnState, iec_state);
  if (iec_atnState != IECAttentionState &&
      (iec_state == IECWaitState || iec_state == IECOutputState))
    return IRQ_HANDLED;

  if (iec_state == IECChannelState) {
    /* Take a break driver 8. We can reach our destination, but we're still a ways away */
    queue_work(iec_writeQ, &iec_writeWork);
    return IRQ_HANDLED;
  }
  
  atn = !digitalRead(IEC_ATN);
  if (atn && iec_atnState == IECAttentionIgnoreState)
    return IRQ_HANDLED;

  abort = 0;

  val = iec_readByte();
  //printk(KERN_NOTICE "IEC: Read: %03x %i %i\n", val, atn, iec_atnState);
  if (val >= 0) {
    if (atn) { 
      val |= DATA_ATN;

      /* Partially processing commands that may need to release bus
	 here because of timing issues. */
      cmd = val & 0xe0;
      dev = val & 0x1f;
      switch (cmd) {
      case IECListenCommand:
	if (dev == IEC_ALLDEV || !iec_openDevices[dev]) {
	  iec_atnState = IECAttentionIgnoreState;
	  if (dev == IEC_ALLDEV)
	    iec_state = IECWaitState;
	  udelay(c64slowdown);
	  iec_releaseBus();
	}
	break;

      case IECTalkCommand:
	if (dev == IEC_ALLDEV || !iec_openDevices[dev]) {
	  iec_atnState = IECAttentionIgnoreState;
	  if (dev == IEC_ALLDEV)
	    iec_state = IECWaitState;
	  udelay(c64slowdown);
	  iec_releaseBus();
	}
	break;
      }
    }

    iec_buffer[iec_inpos] = val;
    iec_inpos = (iec_inpos + 1) % IEC_BUFSIZE;
    queue_work(iec_readQ, &iec_readWork);
  }
  
  return IRQ_HANDLED;
}

static void iec_processData(struct work_struct *work)
{
  int avail;
  int val, atn;
  int cmd, dev;


  for (;;) {
    avail = (IEC_BUFSIZE + iec_inpos - iec_outpos) % IEC_BUFSIZE;
    if (!avail)
      return;
  
    val = iec_buffer[iec_outpos];
    atn = val & DATA_ATN;
    iec_outpos = (iec_outpos + 1) % IEC_BUFSIZE;
    //printk(KERN_NOTICE "IEC: processing data %02x\n", val);

    if (atn) {
      cmd = val & 0xe0;
      dev = val & 0x1f;

      switch (cmd) {
      case IECListenCommand:
	if (dev == IEC_ALLDEV || !iec_openDevices[dev])
	  iec_closeIO(INPUT);
	else {
	  iec_state = IECListenState;
	  iec_newIO(val, INPUT);
	  iec_curDevice = dev;
	}
	break;

      case IECTalkCommand:
	if (dev == IEC_ALLDEV || !iec_openDevices[dev])
	  iec_closeIO(INPUT);
	else {
	  iec_state = IECTalkState;
	  iec_newIO(val, INPUT);
	  iec_curDevice = dev;
	}
	break;

      case IECChannelCommand:
	iec_channelIO(val, INPUT);
	if (iec_state == IECTalkState) {
	  iec_state = IECChannelState;
	  iec_closeIO(INPUT);
	  queue_work(iec_writeQ, &iec_writeWork);
	}
	break;

      case IECFileCommand:
	if (dev == 0x00)
	  iec_closeIO(INPUT);
	else
	  iec_channelIO(val, INPUT);
	iec_curChannel = dev & 0x0f;
	break;
      }
    }
    else {
      iec_appendIO(val, INPUT);
      if (val & DATA_EOI)
	iec_closeIO(INPUT);
    }
  }

  return;
}

int iec_writeByte(int bits)
{
  int len;
  int abort = 0;
  unsigned long flags;


  disable_irq(irq_clk);
  local_irq_save(flags);
  //printk(KERN_NOTICE "IEC: Write: %03x data: %i\n", bits, digitalRead(IEC_DATA));
#if NUMPINS == 3
  pinMode(IEC_CLK, INPUT);
#else
  digitalWrite(IEC_CLKOUT, HIGH);
#endif

  if ((abort = iec_waitForSignal(IEC_DATA, 1, 100000)))
    printk(KERN_NOTICE "IEC: Timeout waiting to send\n");

  if (!abort && (bits & DATA_EOI)) {
    if ((abort = iec_waitForSignal(IEC_DATA, 0, 100000)))
      printk(KERN_NOTICE "IEC: Timeout waiting for EOI ack\n");

    if (!abort && (abort = iec_waitForSignal(IEC_DATA, 1, 100000)))
      printk(KERN_NOTICE "IEC: Timeout waiting for EOI ack finish\n");
  }

#if NUMPINS == 3
  pinMode(IEC_CLK, OUTPUT);
#else
  digitalWrite(IEC_CLKOUT, LOW);
#endif
  local_irq_restore(flags);
  udelay(c64slowdown);
  
  for (len = 0; !abort && len < 8; len++, bits >>= 1) {
    if (iec_state != IECOutputState) {
      abort = 1;
      break;
    }
#if NUMPINS == 3
    if (bits & 1)
      pinMode(IEC_DATA, INPUT);
    else
      pinMode(IEC_DATA, OUTPUT);
#else
    digitalWrite(IEC_DATAOUT, bits & 1);
#endif
    udelay(c64slowdown*2);
#if NUMPINS == 3
    pinMode(IEC_CLK, INPUT);
#else
    digitalWrite(IEC_CLKOUT, HIGH);
#endif
    udelay(c64slowdown*2);
#if NUMPINS == 3
    pinMode(IEC_DATA, INPUT);
    pinMode(IEC_CLK, OUTPUT);
#else
    digitalWrite(IEC_DATAOUT, HIGH);
    digitalWrite(IEC_CLKOUT, LOW);
#endif
  }
  enable_irq(irq_clk);

  if (!abort && (abort = iec_waitForSignal(IEC_DATA, 0, 10000))) {
    printk(KERN_NOTICE "IEC: Timeout waiting for listener ack\n");
    abort = 0;
  }

  udelay(c64slowdown);
  
  return abort;
}

int iec_setupTalker(void)
{
  int abort = 0;


  //printk(KERN_NOTICE "IEC: switching to talk\n");
  abort = iec_waitForSignal(IEC_ATN, 1, 1000000);  
  if (!abort && (abort = iec_waitForSignal(IEC_CLK, 1, 100000)))
    printk(KERN_NOTICE "IEC: Timeout waiting for start of talk\n");

  if (!abort) {
#if NUMPINS == 3
    pinMode(IEC_DATA, INPUT);
    pinMode(IEC_CLK, OUTPUT);
#else
    digitalWrite(IEC_DATAOUT, HIGH);
    digitalWrite(IEC_CLKOUT, LOW);
#endif
    udelay(c64slowdown);
    iec_state = IECOutputState;
  }

  //printk(KERN_NOTICE "IEC: talking %i\n", abort);
  
  return abort;
}

static void iec_sendData(struct work_struct *work)
{
  int abort;
  int val;
  iec_device *device;
  iec_io *io;
  iec_linkedData *data;
  unsigned char *wbuf;
  int pos, len;


  /* FIXME - lock device while sending data so we can send incomplete buffers */
  abort = 0;
  if (iec_state == IECChannelState)
    abort = iec_setupTalker();

  while (iec_state == IECOutputState && !abort) {
    //printk(KERN_NOTICE "IEC: talk state %i %i\n", iec_state, iec_curDevice);
    if (!(device = iec_openDevices[iec_curDevice])) {
      printk(KERN_NOTICE "IEC: device %i not open\n", iec_curDevice);
      return;
    }
    if (!device->out.head) {
      printk(KERN_NOTICE "IEC: device %i has no data\n", iec_curDevice);
      return;
    }

    io = device->out.head;
    while (io && io != device->out.cur && io->header.channel != iec_curChannel)
      io = io->next;
  
    if (!io) {
      printk(KERN_NOTICE "IEC: device %i has no data for channel %i\n",
	     iec_curDevice, iec_curChannel);
      return;
    }

    /* If length is 0 release bus to indicate file not found. No way
       to have a 0 length file? */
    if (!io->header.len) {
      printk(KERN_NOTICE "IEC: file not found\n");
      iec_state = IECWaitState;
      iec_releaseBus();
      iec_unlinkIO(device, io, OUTPUT);
      return;
    }
    
    data = io->data;
    //printk(KERN_NOTICE "IEC: starting data %i len %i\n", io->pos, io->header.len);
    pos = io->pos - sizeof(io->header);
    len = io->header.len;
    while (pos >= IEC_BUFSIZE) {
      //printk(KERN_NOTICE "IEC: next buffer\n");
      data = data->next;
      pos -= IEC_BUFSIZE;
      len -= IEC_BUFSIZE;
    }

    if (len > IEC_BUFSIZE)
      len = IEC_BUFSIZE;
    len -= pos;
    wbuf = &data->data[pos];

    //printk(KERN_NOTICE "IEC: data of length %i from %i\n", len, pos);
    if (len < 0) {
      abort = 1;
      break;
    }
    
    for (pos = 0; !abort && iec_state == IECOutputState && pos < len; pos++) {
      //printk(KERN_NOTICE "IEC: sending %i of %i\n", pos, len);
      val = wbuf[pos];
      if (pos == len - 1 && !data->next) {
	//printk(KERN_NOTICE "IEC: last byte\n");
	val |= DATA_EOI;
      }
      abort = iec_writeByte(val);
    }
    io->pos += len;

    //printk(KERN_NOTICE "IEC: sent entire file %i %i %i\n", pos, len, (int) data->next);
    if (io->pos - sizeof(io->header) == io->header.len && !iec_unlinkIO(device, io, OUTPUT)) {
      printk(KERN_NOTICE "IEC: unable to lock IO\n");
      return;
    }
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
  int minor;


  /* http://www.makelinux.com/ldd3/ */
  /* http://stackoverflow.com/questions/5970595/create-a-device-node-in-code */
  
  mem = request_mem_region(GPIO_BASE, 4096, LABEL_DEVICE);
  gpio = ioremap(GPIO_BASE, 4096);

  pinMode(IEC_ATN, INPUT);
  pinMode(IEC_CLK, INPUT);
  pinMode(IEC_DATA, INPUT);

#if NUMPINS == 3
  digitalWrite(IEC_CLK, LOW);
  digitalWrite(IEC_DATA, LOW);
#else
  pinMode(IEC_CLKOUT, OUTPUT);
  pinMode(IEC_DATAOUT, OUTPUT);

  digitalWrite(IEC_CLKOUT, HIGH);
  digitalWrite(IEC_DATAOUT, HIGH);
#endif
  
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
			    IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
			    LABEL_ATN, LABEL_DEVICE))) {
    printk(KERN_NOTICE "IEC: IRQ Request failure\n");
    goto fail_irqatn;
  }

  if ((result = request_irq(irq_clk, (irq_handler_t) iec_handleCLK, 
			    IRQF_TRIGGER_RISING, LABEL_CLK, LABEL_DEVICE))) {
    printk(KERN_NOTICE "IEC: IRQ Request failure\n");
    goto fail_irqclk;
  }

  if ((result = alloc_chrdev_region(&iec_devno, 0, IEC_ALLDEV, LABEL_DEVICE)) < 0) {
    printk(KERN_NOTICE "IEC: cannot register device\n");
    goto fail_chrdev;
  }

  cdev_init(&iec_cdev, &iec_fops);
  iec_cdev.owner = THIS_MODULE;
  if ((result = cdev_add(&iec_cdev, iec_devno, IEC_ALLDEV))) {
    printk(KERN_NOTICE "IEC: failed to add device\n");
    goto fail_cdevadd;
  }

  iec_class = class_create(THIS_MODULE, LABEL_DEVICE);
  if (IS_ERR(iec_class)) {
    printk(KERN_NOTICE "IEC: faile to create class\n");
    goto fail_class;
  }

  device_create(iec_class, NULL, MKDEV(MAJOR(iec_devno), 0), NULL, "iec%i", 0);
  for (minor = IEC_FIRSTDEV; minor < IEC_ALLDEV; minor++)
    device_create(iec_class, NULL, MKDEV(MAJOR(iec_devno), minor), NULL, "iec%i", minor);
  
  printk(KERN_NOTICE "IEC module loaded. Major: %i\n", MAJOR(iec_devno));

  iec_writeQ = create_singlethread_workqueue(LABEL_WRITE);
  iec_readQ = create_singlethread_workqueue(LABEL_READ);
  
  return 0;

 fail_class:
  cdev_del(&iec_cdev);
 fail_cdevadd:
  unregister_chrdev_region(iec_devno, IEC_ALLDEV);
 fail_chrdev:
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
  return result;
}

void iec_cleanupDevice(int minor)
{
  iec_device *device;
  iec_io *io, *ioNext;
  iec_linkedData *data, *next;


  if ((device = iec_openDevices[minor])) {
    if (down_interruptible(&device->lock)) {
      printk(KERN_NOTICE "IEC: unable to cleanup device %i\n", minor);
      return;
    }

    iec_openDevices[minor] = NULL;
    up(&device->lock);
    io = device->in.head;
    while (io) {
      ioNext = io->next;
      data = io->data;
      while (data) {
	next = data->next;
	kfree(data);
	data = next;
      }
      kfree(io);
      io = ioNext;
    }

    io = device->out.head;
    while (io) {
      ioNext = io->next;
      data = io->data;
      while (data) {
	next = data->next;
	kfree(data);
	data = next;
      }
      kfree(io);
      io = ioNext;
    }
    kfree(device);
  }
  
  return;
}
    
void iec_cleanup(void)
{
  int minor;


  destroy_workqueue(iec_writeQ);
  destroy_workqueue(iec_readQ);
  device_destroy(iec_class, MKDEV(MAJOR(iec_devno), 0));
  for (minor = 4; minor < IEC_ALLDEV; minor++) {
    device_destroy(iec_class, MKDEV(MAJOR(iec_devno), minor));
    iec_cleanupDevice(minor);
  }
  class_destroy(iec_class);
  cdev_del(&iec_cdev);
  unregister_chrdev_region(iec_devno, IEC_ALLDEV);
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
  int minor = iminor(inode);
  iec_device *device;


  printk(KERN_NOTICE "IEC: opening device %i\n", minor);
  device = iec_openDevices[minor];

  /* FIXME - allow more than one process to open at once */
  if (device) {
    printk(KERN_NOTICE "IEC: another process already has device %i open\n", minor);
    return 0;
  }

  device = iec_openDevices[minor] = kmalloc(sizeof(iec_device), GFP_KERNEL);    
  device->in.head = device->in.tail = device->in.cur = NULL;
  device->out.head = device->out.tail = device->out.cur = NULL;
  device->flags = filp->f_flags;
  sema_init(&device->lock, 1);
  iec_deviceCount++;

  filp->private_data = device;
  return 0;
}

int iec_close(struct inode *inode, struct file *filp)
{
  int minor = iminor(inode);


  iec_cleanupDevice(minor);
  iec_deviceCount--;
  return 0;
}

unsigned int iec_poll(struct file *filp, poll_table *wait)
{
  int avail = 0;
  iec_device *device = filp->private_data;
  iec_io *io;
  unsigned int pollMask;


  io = device->in.head;
  poll_wait(filp, &iec_wait, wait);
  if (io && io != device->in.cur)
    avail = (io->header.len + 4) - io->pos;
  
  pollMask = POLLOUT | POLLWRNORM;
  if (avail)
    pollMask |= POLLIN | POLLRDNORM;
  
  return pollMask;
}

ssize_t iec_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  unsigned long remaining;
  int avail = 0;
  iec_device *device = filp->private_data;
  iec_io *io;
  unsigned char *wbuf;


  do {
    io = device->in.head;
    if (io && io != device->in.cur) {
      avail = (io->header.len + 4) - io->pos;
#if 0
      if (io->pos == io->header.len + sizeof(io->header) && !avail)
	return 0;
#endif
    }
    
    if (!avail && wait_event_interruptible(iec_wait, iec_readAvail))
      return -ERESTARTSYS;
    iec_readAvail = 0;
  } while (!avail);
  
  if (count > avail)
    count = avail;

  if (io->pos < sizeof(io->header)) {
    wbuf = (unsigned char *) &io->header;
    wbuf += io->pos;
    if (count > sizeof(io->header) - io->pos)
      count = sizeof(io->header) - io->pos;
  }
  else {
    iec_linkedData *data;
    int pos, len;


    data = io->data;
    pos = io->pos - sizeof(io->header);
    len = io->header.len;
    while (pos > IEC_BUFSIZE) {
      data = data->next;
      pos -= IEC_BUFSIZE;
      len -= IEC_BUFSIZE;
    }

    if (count > len - pos)
      count = len - pos;
    if (count > IEC_BUFSIZE - pos)
      count = IEC_BUFSIZE - pos;
    wbuf = &data->data[pos];
  }

  remaining = copy_to_user(buf, wbuf, count);
  count -= remaining;
  io->pos += count;

  if (io->pos == io->header.len + sizeof(io->header) &&
      !iec_unlinkIO(device, io, INPUT))
    return -ERESTARTSYS;
  
  *f_pos += count;
  return count;
}

ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  iec_device *device = filp->private_data;
  iec_io *io;
  unsigned long remaining;
  unsigned char *wbuf;
  char sbuf[256];
  int minor = iminor(filp->f_path.dentry->d_inode);
  size_t len, total;


  //printk(KERN_NOTICE "IEC: request to write %i bytes\n", count);
  total = count;
  while (total > 0) {
    len = count;
    
    if (!device->out.cur) {
      //printk(KERN_NOTICE "IEC: starting output buffer for %i\n", minor);
      iec_newIO(minor, OUTPUT);
    }

    io = device->out.cur;

    if (io->pos < sizeof(io->header)) {
      wbuf = (unsigned char *) &io->header;
      wbuf += io->pos;
      if (len > sizeof(io->header) - io->pos)
	len = sizeof(io->header) - io->pos;

      remaining = copy_from_user(wbuf, buf, len);
      len -= remaining;
      io->pos += len;
    }
    else {
      if (len > sizeof(sbuf))
	len = sizeof(sbuf);
      if (len > (io->header.len + sizeof(io->header)) - io->pos)
	len = (io->header.len + sizeof(io->header)) - io->pos;
      remaining = copy_from_user(sbuf, buf, len);
      len -= remaining;
      iec_appendData(sbuf, len, OUTPUT);
      //printk(KERN_NOTICE "IEC: added %i bytes to output buffer for %i\n", len, minor);
    }

    /* FIXME - lock device while sending data so we can send incomplete buffers */
    //printk(KERN_NOTICE "IEC: pos %i len %i\n", io->pos, io->header.len);
    if (io->pos == io->header.len + sizeof(io->header)) {
      //printk(KERN_NOTICE "IEC: read all data for %i len %i\n", minor, io->header.len);
      iec_closeIO(OUTPUT);
    }

    total -= len;
  }
  
  *f_pos += count;
  return count;
}

module_init(iec_init);
module_exit(iec_cleanup);

/****************************************************************************/
/* Module licensing/description block.                                      */
/****************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
