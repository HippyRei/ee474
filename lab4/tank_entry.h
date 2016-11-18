#ifndef _TANK_ENTRY_H
#define _TANK_ENTRY_H

#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define ACD_SLOTS_PATH "/sys/devices/bone_capemgr.9/slots"
#define PWM_SLOTS_PATH "/sys/devices/bone_capemgr.9/slots"

#define SWITCH_GPIO_NUM 20
#define SWITCH_VAL "/sys/class/gpio/gpio20/value"
#define SWITCH_DIR "/sys/class/gpio/gpio20/direction"

#define LED_GPIO_NUM 61
#define LED_VAL "/sys/class/gpio/gpio61/value"
#define LED_DIR "/sys/class/gpio/gpio61/direction"

#define A_SLOT "bone_pwm_P9_14"
#define B_SLOT "bone_pwm_P9_16"

#define AIN1 "/sys/devices/ocp.3/helper.17/AIN1"

void setPin(char *, char *);
void activateGPIO(int);
void enable_adc();
void initializePWMSlots();
void activatePWM(char * pwm);

#endif //_TANK_ENTRY_H
