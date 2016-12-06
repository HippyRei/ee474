#ifndef BT_LISTENER_H
#define BT_LISTENER_H

#define _BSD_SOURCE
#define _POSIX_SOURCE 1

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
//#include <sys/signal.h>
#include <sys/types.h>

#define DEVPATH "/sys/devices/bone_capemgr.9/slots"

#define BAUDRATE B115200

#define LEN 12

#define UART1 "/dev/ttyO2"

void enable_UART1();
void signal_handler_IO (int status);   /* definition of signal handler */

#endif //BT_LISTENER_H
