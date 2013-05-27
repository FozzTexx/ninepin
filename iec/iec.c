#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/interrupt.h>
#include <linux/gpio.h>

#define DRIVER_AUTHOR	"Chris Osborn <fozztexx@fozztexx.com>"
#define DRIVER_DESC	"Commodore IEC serial driver"

#define IEC_ATN		25
#define IEC_CLK		8
#define IEC_DATA	7

#define IEC_DESC	"Clock pin for CBM IEC"
#define DEVICE_DESC	"some_device"

short int iec_irq = 0;
int iec_major = 60;
char *iec_buffer;

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

static irqreturn_t iec_handler(int irq, void *dev_id, struct pt_regs *regs) {
  unsigned long flags;


  // disable hard interrupts (remember them in flag 'flags')
  local_irq_save(flags);

  printk(KERN_NOTICE "Interrupt [%d] for device %s was triggered !.\n",
	 irq, (char *) dev_id);

  // restore hard interrupts
  local_irq_restore(flags);

  return IRQ_HANDLED;
}

void iec_config(void) {
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
		  IRQF_TRIGGER_FALLING, IEC_DESC, DEVICE_DESC)) {
    printk("Irq Request failure\n");
    return;
  }

  return;
}

/****************************************************************************/
/* This function releases interrupts.                                       */
/****************************************************************************/
void iec_release(void) {
  free_irq(iec_irq, IEC_DESC);
  gpio_free(IEC_CLK);
  return;
}

/****************************************************************************/
/* Module init / cleanup block.                                             */
/****************************************************************************/
int iec_init(void) {
  int result;


  if ((result = register_chrdev(iec_major, "iec", &iec_fops)) < 0) {
    printk(KERN_NOTICE "IEC: cannot obtain major number %i\n", iec_major);
    return result;
  }

  if (!(iec_buffer = kmalloc(10, GFP_KERNEL))) {
    printk(KERN_NOTICE "IEC: failed to allocate buffer\n");
    return -ENOMEM;
  }

  /* FIXME - don't just say loaded, check iec_config results */
  printk(KERN_NOTICE "IEC module loaded\n");
  iec_config();
  return 0;
}

void iec_cleanup(void) {
  unregister_chrdev(iec_major, "iec");
  kfree(iec_buffer);
  iec_release();
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
