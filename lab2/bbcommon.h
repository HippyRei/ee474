#ifndef BBCOMMON_H
#define BBCOMMON_H

#include <stdio.h>
#include <unistd.h>

#define GPIO20_VAL "../../sys/class/gpio/gpio20/value"
#define DB0_VAL "../../sys/class/gpio/gpio66/value"
#define DB1_VAL "../../sys/class/gpio/gpio69/value"
#define DB2_VAL "../../sys/class/gpio/gpio45/value"
#define DB3_VAL "../../sys/class/gpio/gpio47/value"
#define DB4_VAL "../../sys/class/gpio/gpio67/value"
#define DB5_VAL "../../sys/class/gpio/gpio68/value"
#define DB6_VAL "../../sys/class/gpio/gpio44/value"
#define DB7_VAL "../../sys/class/gpio/gpio26/value"
#define RS_VAL "../../sys/class/gpio/gpio49/value"
#define RW_VAL "../../sys/class/gpio/gpio117/value"
#define E_VAL "../../sys/class/gpio/gpio115/value"

#define GPIO20_DIR "../../sys/class/gpio/gpio20/direction"
#define DB0_DIR "../../sys/class/gpio/gpio66/direction"
#define DB1_DIR "../../sys/class/gpio/gpio69/direction"
#define DB2_DIR "../../sys/class/gpio/gpio45/direction"
#define DB3_DIR "../../sys/class/gpio/gpio47/direction"
#define DB4_DIR "../../sys/class/gpio/gpio67/direction"
#define DB5_DIR "../../sys/class/gpio/gpio68/direction"
#define DB6_DIR "../../sys/class/gpio/gpio44/direction"
#define DB7_DIR "../../sys/class/gpio/gpio26/direction"
#define RS_DIR "../../sys/class/gpio/gpio49/direction"
#define RW_DIR "../../sys/class/gpio/gpio117/direction"
#define E_DIR "../../sys/class/gpio/gpio115/direction"

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

// Activates the GPIO associated with gnum
void activateGPIO(int gnum);

// Writes flag to the file associated with path
void setGPIO(char * path, char * flag);

void isetGPIO(char * path, int flag);

void initializeLCD();

#endif
