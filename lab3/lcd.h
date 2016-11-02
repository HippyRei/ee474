#ifndef LCD_H_
#define LCD_H_

#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/gpio.h>
#include <linux/delay.h>            //for time delays
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>

#define SHIFT_DEL 100

#define SHIFT_DISP_L 0x18
#define SHIFT_DISP_R 0x1C
#define SHIFT_CURS_L 0x10
#define SHIFT_CURS_R 0x14

#define CLEAR_DISP 0x1

static dev_t dev_num;

struct fake_device {
	char data[100];
	struct semaphore sem;
} virtual_device;

static struct cdev* mcdev;

static int  lcd_open(struct inode*, struct file*);
static int lcd_close(struct inode*, struct file *);
static ssize_t lcd_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops = {
   .owner = THIS_MODULE,
   .write = lcd_write,
   .open = lcd_open,
   .release = lcd_close
};

#endif // (LCD_H_)
