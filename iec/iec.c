/* Copyright 2013 by Chris Osborn <fozztexx@fozztexx.com>
 *
 * $Id$
 */

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

#define DRIVER_AUTHOR	"Chris Osborn <fozztexx@fozztexx.com>"
#define DRIVER_DESC	"Commodore IEC serial driver"

#define IEC_ATN		25
#define IEC_CLK		8
#define IEC_DATA	7

#define IEC_DESC	"Clock pin for CBM IEC"
#define DEVICE_DESC	"cbm-iec"

#define BCM2708_PERI_BASE   0x20000000
#define GPIO_BASE  (BCM2708_PERI_BASE + 0x200000)

#define INPUT	0
#define OUTPUT	1

#define digitalRead(pin)	(*(gpio + 13) & (1 << (pin & 31)))
#define digitalWrite(pin, val)	(*(gpio + 7 + (val ? 0 : 3)) = 1 << (pin & 31))
#define pinMode(pin, mode)	(*(gpio + pin / 10) &= ~(7 << (pin % 10) * 3) | \
				 (mode << (pin % 10) * 3))

static short int iec_irq = 0;
static int iec_major = 60;
static char *iec_buffer;
static volatile uint32_t *gpio;

int iec_open(struct inode *inode, struct file *filp);
int iec_close(struct inode *inode, struct file *filp);
ssize_t iec_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

struct file_operations iec_fops = {
 read: iec_read,
 write: iec_write,
 open: iec_open,
 release: iec_close
};

static irqreturn_t iec_handler(int irq, void *dev_id, struct pt_regs *regs)
{
  unsigned long flags;


  // disable hard interrupts (remember them in flag 'flags')
  local_irq_save(flags);

  /* FIXME - read bits and buffer data */
  printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered !.\n",
	 irq, (char *) dev_id);

  // restore hard interrupts
  local_irq_restore(flags);

  return IRQ_HANDLED;
}

void iec_config(void)
{
  if (gpio_request(IEC_CLK, IEC_DESC)) {
    printk("GPIO request faiure: %s\n", IEC_DESC);
    return;
  }

  if ((iec_irq = gpio_to_irq(IEC_CLK)) < 0) {
    printk("GPIO to IRQ mapping faiure %s\n", IEC_DESC);
    return;
  }

  printk(KERN_NOTICE "Mapped int %d\n", iec_irq);

  if (request_irq(iec_irq, (irq_handler_t) iec_handler, 
		  IRQF_TRIGGER_RISING, IEC_DESC, DEVICE_DESC)) {
    printk("Irq Request failure\n");
    return;
  }

  return;
}

/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
int iec_init(void)
{
  int result;
  struct resource *mem;


  mem = request_mem_region(GPIO_BASE, 4096, DEVICE_DESC);
  gpio = ioremap(GPIO_BASE, 4096);
   
  if ((result = register_chrdev(iec_major, DEVICE_DESC, &iec_fops)) < 0) {
    printk(KERN_NOTICE "IEC: cannot obtain major number %i\n", iec_major);
    return result;
  }

  if (!(iec_buffer = kmalloc(10, GFP_KERNEL))) {
    printk(KERN_NOTICE "IEC: failed to allocate buffer\n");
    return -ENOMEM;
  }

  /* FIXME - don't just say loaded, check iec_config results */
  iec_config();
  printk(KERN_NOTICE "IEC module loaded\n");
  
  return 0;
}

void iec_cleanup(void)
{
  unregister_chrdev(iec_major, DEVICE_DESC);
  kfree(iec_buffer);

  free_irq(iec_irq, IEC_DESC);
  gpio_free(IEC_CLK);

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

  
  remaining = copy_to_user(buf, iec_buffer, 1);

  if (*f_pos == 0) {
    *f_pos += 1;
    return 1;
  }

  return 0;
}

ssize_t iec_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  const char *tmp;
  unsigned long remaining;
  

  tmp = buf + count - 1;
  remaining = copy_from_user(iec_buffer, tmp, 1);
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
