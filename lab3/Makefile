export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
ccflags-y := -std=gnu99

obj-m += lcd.o

# Kernel source directory
KDIR = ../../bb-kernel/KERNEL
PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
