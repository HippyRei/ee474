/*
 * @file    hello.c
 * @author  Group10
 * @date    26 October 2016
 * @version 0.1
 * @brief   
 * 
 * 
 * 
*/
 
#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/gpio.h>
#include <linux/delay.h>            //for time delays
#define SHIFT_DEL 100
 
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

/*
void clock_cycle() {
  gpio_set_value(gpios[3].gpio, 1);
  ndelay(SHIFT_DEL);
  gpio_set_value(gpios[3].gpio, 0);
  ndelay(SHIFT_DEL);
}
*/

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
static int __init helloBBB_init(void){
   printk(KERN_INFO "EBB: Hello %s from the BBB LKM!\n", name);

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
 
/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit helloBBB_exit(void){
   printk(KERN_INFO "EBB: Goodbye %s from the BBB LKM!\n", name);
   gpio_free_array(gpios, 8);
}
 
/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(helloBBB_init);
module_exit(helloBBB_exit);
