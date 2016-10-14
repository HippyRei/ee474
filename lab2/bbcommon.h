#ifndef BBCOMMON_H
#define BBCOMMON_H

#include <stdio.h>

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

// Activates the GPIO associated with gnum
void activateGPIO(int gnum);

// Writes flag to the file associated with path
void setGPIO(char * path, char * flag);

#endif
