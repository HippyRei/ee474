#ifndef BT_LISTENER_H
#define BT_LISTENER_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <termios.h>

#define DEVPATH "/sys/devices/bone_capemgr.9/slots"

#define BAUDRATE B115200

#define UART1 "/dev/tty01"

void enable_UART1();

#endif //BT_LISTENER_H
