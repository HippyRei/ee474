#ifndef _TANK_H_LAB_4
#define _TANK_H_LAB_4

#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#define AIN1 "/sys/devices/ocp.3/helper.15/AIN1"
#define AIN2 "/sys/devices/ocp.3/helper.15/AIN2"
#define AIN3 "/sys/devices/ocp.3/helper.15/AIN3"
#define AIN4 "/sys/devices/ocp.3/helper.15/AIN4"

#define FREQUENCY 10
#define SIG_USER 20

#endif // _TANK_H_LAB_4
