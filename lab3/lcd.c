#include "lcd.h"

// LKM that acts as a simple driver for the LCD by interacting through GPIO pins
// and a shift register. Text and commands can be written to the LCD by writing 
// to the dev file /dev/lcd

// initialize, exit, open, close and write code modeled/obtained from new_char.c provided on the class website.
// structure of a kernel module modeled from Derek Molloy: http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/
 
MODULE_LICENSE("GPL");                     // General Public License
MODULE_AUTHOR("Group10");                  // us, group10 - self explanatory
MODULE_DESCRIPTION("Simple LCD Driver.");  // description
MODULE_VERSION("10x");                     // module version
 
static char *name = "lcd";                 // name of the LKM in the system when it is registered
module_param(name, charp, S_IRUGO); 
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log"); 

// GPIO pins 
static struct gpio gpios[] = {
  { 115, GPIOF_OUT_INIT_LOW, "E" },      // enable 
  { 49, GPIOF_OUT_INIT_LOW, "RS" },      // register select
  { 112, GPIOF_OUT_INIT_LOW, "R/W" },    // read/write
  { 26, GPIOF_OUT_INIT_LOW, "SRCLK" },   // shift register clock
  { 66, GPIOF_OUT_INIT_LOW, "SRCLR" },   // !shift register clear
  { 44, GPIOF_OUT_INIT_LOW, "RCLK" },    // register clock
  { 68, GPIOF_OUT_INIT_LOW, "OE" },      // !output enable
  { 67, GPIOF_OUT_INIT_LOW, "SER" },     // data
};

// Set pins of the LCD (through the shift register) to passed value.
void setPins(int config) {
  int msk;
  gpio_set_value(gpios[4].gpio, 0);      //clear shift register.
  ndelay(SHIFT_DEL);
  gpio_set_value(gpios[4].gpio, 1);      //disable clear shift registers.

  gpio_set_value(gpios[3].gpio, 0);      //set clock to 0
  gpio_set_value(gpios[5].gpio, 0);      //set clock to 0
  gpio_set_value(gpios[6].gpio, 1);      //disable output
  
  msk = 0x80;
  for (int i = 0; i < 8; i++) {          //set pins on register
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

  gpio_set_value(gpios[6].gpio, 0);      //enable output
}

// Flip the enable pin on the LCD
void flipE(void) {
  unsigned e = gpios[0].gpio;
  gpio_set_value(e, 1);
  ndelay(500);
  gpio_set_value(e, 0);
}

// Write a command to the GPIO instruction register.
void writeCommand(int config) {
  unsigned rs = gpios[1].gpio;
  unsigned rw = gpios[2].gpio;

  gpio_set_value(rw, 0);
  gpio_set_value(rs, 0);
  
  setPins(config);
  flipE();

  udelay(750);
}

// Write a character to the LCD.
void writeChar(char c) {
  unsigned rs = gpios[1].gpio;
  unsigned rw = gpios[2].gpio;

  gpio_set_value(rw, 0);
  gpio_set_value(rs, 1);
  
  
  setPins((int) c);
  flipE();
  
  udelay(750);
}

// Write a string to the LCD
void writeString(char * str) {
  char * cur = str;
  while (*cur) {
    writeChar(*cur);
    cur++;
  }
}
 
// LCD LKM initialization function
// initialization code modeled from new_char.c provided on the class website.
static int __init lcd_init(void){
  int ret;
  printk(KERN_INFO "Initializing LCD\n");
  
  if (alloc_chrdev_region(&dev_num, 0, 1, name) < 0) {
    printk(KERN_ALERT "lcd_device: Failed to allocate a major number\n");
    return 0;
  }
  printk(KERN_INFO "mknod /dev/lcd c %d %d\n", MAJOR(dev_num), MINOR(dev_num)); //print major and minor number.

  mcdev = cdev_alloc();
  mcdev->ops = &fops;
  mcdev->owner = THIS_MODULE;

  ret = cdev_add(mcdev, dev_num, 1);
  if (ret < 0) {
    printk(KERN_ALERT "lcd_device: unable to add cdev to kernel\n");
    return ret;
  }

  // Initialize the LCD with commands
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

// LCD exit function
// exit code modeled from new_char.c provided on the class website.
static void __exit lcd_exit(void){
  printk(KERN_INFO "Exiting lcd\n");
  cdev_del(mcdev);
  unregister_chrdev_region(dev_num, 1);  //unregister device
  gpio_free_array(gpios, 8);             //release GPIOs
}

// Open the LCD device 
// open code modeled from new_char.c provided on the class website.
int lcd_open(struct inode *inode, struct file* filp) {
  if (down_interruptible(&virtual_device.sem) != 0) {                      //check semaphore
    printk(KERN_ALERT "lcd_device: could not lock device during open\n");  //fail to open
    return -1;
  }
  printk(KERN_INFO "lcd_device: device opened\n");                         //success
  return 0;
}

// Write a command or text to the LCD device
// open code modeled from new_char.c provided on the class website.
ssize_t lcd_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* curOffset) {
  ssize_t res;
  printk(KERN_INFO "lcd_device: writing to device...\n");

  if (bufCount > 100) {
    bufCount = 100;
  }
  
  res = copy_from_user(virtual_device.data, bufSource, bufCount);
  if ((int) virtual_device.data[bufCount - 1] == 10) {    //input from bash, ensure string is null terminated
    virtual_device.data[bufCount - 1] = '\0';
  } else {                                                //ensure string is null terminated
    virtual_device.data[bufCount] = '\0';
  }
  if (virtual_device.data[0] == '/') {                    //write command to LCD
    if (!strcmp(virtual_device.data, "/clear")) {         //clear
      writeCommand(CLEAR_DISP);
    } else if (!strcmp(virtual_device.data, "/sdl")) {    //shift display left
      writeCommand(SHIFT_DISP_L);
    } else if (!strcmp(virtual_device.data, "/sdr")) {    //shift display right
      writeCommand(SHIFT_DISP_R);
    } else if (!strcmp(virtual_device.data, "/scl")) {    //shift cursor left
      writeCommand(SHIFT_CURS_L);
    } else if (!strcmp(virtual_device.data, "/scr")) {    //shift cursor right
      writeCommand(SHIFT_CURS_R);
    } else if (!strcmp(virtual_device.data, "/bl")) {     //move to bottom line
      writeCommand(0x80 | 0x40);
    } else if (!strcmp(virtual_device.data, "/tl")) {     //move to top line
      writeCommand(0x80 | 0x00);
    } else if (!strcmp(virtual_device.data, "/b")) {      //backspace
      writeCommand(SHIFT_CURS_L);
      writeChar(' ');
      writeCommand(SHIFT_CURS_L);
    }
  } else {                                                //write string to LCD
    writeString(virtual_device.data);
  }

  return bufCount - res;                                  //number of characters written
}

// Called when user wants to get info from device file
ssize_t lcd_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
	printk(KERN_ALERT "Read is an unsupported operation for this device\n");
	return -1;
}

// Close the LCD device
// close code modeled from new_char.c provided on the class website.
int lcd_close(struct inode* inode, struct  file *filp) {
  up(&virtual_device.sem);                                //release semaphore
  printk(KERN_INFO "lcd_device: closing device\n");
  return 0;	
}

module_init(lcd_init);
module_exit(lcd_exit);
