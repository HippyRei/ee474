#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define AIN1 /sys/devices/ocp.3/helper.15/AIN1
#define AIN2 /sys/devices/ocp.3/helper.15/AIN2
#define AIN3 /sys/devices/ocp.3/helper.15/AIN3
#define AIN4 /sys/devices/ocp.3/helper.15/AIN4

#define FREQUENCY 10

struct sigaction sa;
struct sigevent se;

struct timespec ts; 

struct itimerspec itimer;

timer_t timerid;

//timer interrupt handler
void timer_handler(int whatever)
{
  /*send user defined signal (SIG_USER) to the process with PID
    kill() doesn't neccessarily mean to terminate another process,
    and in this case, it means to send signal "SIG_USER" to the
    process with "pid"
  */
  if(kill(pid,SIG_USER) != 0){ 
    printf("Can't send msg\n");
    exit(0);
  }
  printf("Signal sent\n");
}

int main() {
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &timer_handler;
  sigaction (SIGVTALRM, &sa, NULL);
  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIGTVALRM;
  timer_create(CLOCK_REALTIME, se, &timerid);
  ts.tv_sec = 0;
  ts.tv_nsec = 1000000000/FREQUENCY;
  itimer.it_interval = ts;
  itimer.it_value = ts;
  timer_settime(timerid, 0, &itimer, NULL);
    
  
    
  while(1) {
    
  }

}
