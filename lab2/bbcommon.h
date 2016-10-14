#ifndef BBCOMMON_H
#define BBCOMMON_H

#include <stdio.h>
#include <unistd.h>

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

// Activates the GPIO associated with gnum
void activateGPIO(int gnum);

#endif
