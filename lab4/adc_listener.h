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

#define ACD_SLOTS_PATH "/sys/devices/bone_capemgr.9/slots"

#define AIN1 "/sys/devices/ocp.3/helper.17/AIN1"
#define AIN2 "/sys/devices/ocp.3/helper.17/AIN2"
#define AIN3 "/sys/devices/ocp.3/helper.17/AIN3"
#define AIN4 "/sys/devices/ocp.3/helper.17/AIN4"

#define FREQUENCY 20
#define SIG_USER 20

#define REPORT_T 10000000

#define LEN 12

void enable_acd();

int read_acd(char * path);


#endif // _TANK_H_LAB_4
