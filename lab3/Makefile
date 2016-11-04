export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
ccflags-y := -std=gnu99

obj-m += lcd.o

# Kernel source directory
KDIR = ../../bb-kernel/KERNEL
PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

minesweeper: minesweeper.c minesweeper.h
	gcc -std=c99 -o minesweeper.exe minesweeper.c

getmoney: getmoney.c getmoney.h
	gcc -std=c99 -o getmoney.exe getmoney.c

input: input.c input.h
	gcc -std=c99 -o input.exe input.c

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -rf *.exe
