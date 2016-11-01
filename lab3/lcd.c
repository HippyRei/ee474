#include "lcd.h"
 
MODULE_LICENSE("GPL");              ///< The license type -- this affects runtime behavior
MODULE_AUTHOR("Group10");      ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Simple LCD Driver.");  ///< The description -- see modinfo
MODULE_VERSION("10x");              ///< The version of the module
 
static char *name = "world";        ///< An example LKM argument -- default value is "world"
module_param(name, charp, S_IRUGO); ///< Param desc. charp = char ptr, S_IRUGO can be read/not changed
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");  ///< parameter description

static struct gpio gpios[] = {
  { 115, GPIOF_OUT_INIT_LOW, "E" },
  { 49, GPIOF_OUT_INIT_LOW, "RS" },
  { 112, GPIOF_OUT_INIT_LOW, "R/W" },
  { 26, GPIOF_OUT_INIT_LOW, "SRCLK" },
  { 66, GPIOF_OUT_INIT_LOW, "SRCLR" },
  { 44, GPIOF_OUT_INIT_LOW, "RCLK" },
  { 68, GPIOF_OUT_INIT_LOW, "OE" },
  { 67, GPIOF_OUT_INIT_LOW, "SER" },
};

void setPins(int config) {
  int msk;
  gpio_set_value(gpios[4].gpio, 0);
  ndelay(SHIFT_DEL);
  gpio_set_value(gpios[4].gpio, 1);

  gpio_set_value(gpios[3].gpio, 0);
  gpio_set_value(gpios[5].gpio, 0);
  gpio_set_value(gpios[6].gpio, 1);
  
  msk = 0x80;
  for (int i = 0; i < 8; i++) {
    gpio_set_value(gpios[7].gpio, !(!(config & msk)));
    gpio_set_value(gpios[3].gpio, 1);
    ndelay(SHIFT_DEL);
    gpio_set_value(gpios[3].gpio, 0);
    config *= 2;
  }
  ndelay(SHIFT_DEL);
  gpio_set_value(gpios[5].gpio, 1);
  ndelay(SHIFT_DEL);
  gpio_set_value(gpios[5].gpio, 0);

  gpio_set_value(gpios[6].gpio, 0);
}

void flipE(void) {
  unsigned e = gpios[0].gpio;
  gpio_set_value(e, 1);
  ndelay(500);
  gpio_set_value(e, 0);
}

// Write a command to the GPIO instruction register
void writeCommand(int config) {
  unsigned rw = gpios[1].gpio;
  unsigned rs = gpios[2].gpio;

  gpio_set_value(rw, 0);
  gpio_set_value(rs, 0);
  
  setPins(config);
  flipE();

  udelay(750);
  //msleep(1);
}
 
/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init lcd_init(void){
  int ret;
  printk(KERN_INFO "EBB: Initializing LCD\n");
  
  if (alloc_chrdev_region(&dev_num, 0, 1, name) < 0) {
    printk(KERN_ALERT "new_char: Failed to allocate a major number\n");
    return 0;
  }
  printk(KERN_INFO "mknod /dev/lcd c %d %d\n", MAJOR(dev_num), MINOR(dev_num));

  mcdev = cdev_alloc();
  mcdev->ops = &fops;
  mcdev->owner = THIS_MODULE;

  ret = cdev_add(mcdev, dev_num, 1);
  if (ret < 0) {
    printk(KERN_ALERT "new_char: unable to add cdev to kernerl\n");
    return ret;
  }

  // Initialize SEMAPHORE
  sema_init(&virtual_device.sem, 1);

  gpio_request_array(gpios, 8);

  // Wait 15 ms after power is turned on
  msleep(15);

  // Write first function set command
  setPins(0x30);
   
  flipE();

  msleep(5);

  // Write second function set command
  flipE();

  udelay(100);
  
  // Write third function set command
  flipE();

  msleep(5);

  // Set to 2 lines, 5x10 font
  writeCommand(0x38);

  // Display off
  writeCommand(0x8);

  // Clear display
  writeCommand(0x1);

  // Entry Mode Set
  writeCommand(0x6);

  // Display on
  writeCommand(0xF);
   
  return 0;
}

static void __exit lcd_exit(void){
  printk(KERN_INFO "EBB: Exiting lcd\n");
  cdev_del(mcdev);
  unregister_chrdev_region(dev_num, 1);
  gpio_free_array(gpios, 8);
}

int lcd_open(struct inode *inode, struct file* filp) {
  if (down_interruptible(&virtual_device.sem) != 0) {
    printk(KERN_ALERT "new_char: could not lock device during open\n");
    return -1;
  }
  printk(KERN_INFO "new_char: device opened\n");
  return 0;
}

int lcd_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* curOffset) {
  int res;
  printk(KERN_INFO "new_char: writing to device...\n");
  res = copy_from_user(virtual_device.data, bufSource, bufCount);

  printk(KERN_INFO "%s\n", virtual_device.data);
  printk(KERN_INFO "Got here.\n");
  
  return res;
}

int lcd_close(struct inode* inode, struct  file *filp) {
  up(&virtual_device.sem);
  printk(KERN_INFO "new_char, closing device\n");
  return 0;	
}

module_init(lcd_init);
module_exit(lcd_exit);
