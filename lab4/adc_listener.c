#include "adc_listener.h"

struct sigaction sa;
struct sigevent se;

struct timespec ts; 

struct itimerspec itimer;

timer_t timerid;

int tot;
int s;

pid_t pid;

//timer interrupt handler
void timer_handler(int signum) {
  /*send user defined signal (SIG_USER) to the process with PID
    kill() doesn't neccessarily mean to terminate another process,
    and in this case, it means to send signal "SIG_USER" to the
    process with "pid"
  */
  /*
  if(kill(pid, SIG_USER) != 0) { 
    printf("Can't send msg\n");
    exit(0);
  }
  printf("Signal sent\n");
  */

  tot += read_acd(AIN1);
  s++;

  if (s == FREQUENCY - 1) {
    if (tot / s >= 1000) {
      printf("You're too close!\n");

      //send signal
      kill(pid, SIGUSR1);
      
    }
    tot = 0;
    s = 0;
  }
}


void enable_acd() {
   // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(ACD_SLOTS_PATH, "w");
  }

  // Enable acd ports so we can read from them
  fprintf(f, "cape-bone-iio");
  fclose(f);
}

int read_acd(char *path) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "r");
  }

  // Buffer to store contents of ACD pin
  char buf[101];
  
  int b_len = fread(buf, 1, 100, f);
  buf[100] = 0;
  
  fclose(f);

  return atoi(buf);
}

int main() {
  tot = 0;
  s = 0;
  
  enable_acd();

  //get PID of tank process
  //from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
  char line[LEN];
  FILE *cmd = popen("pgrep -f tank.exe", "r");

  fgets(line, LEN, cmd);
  pid = strtoul(line, NULL, 10);

  pclose(cmd);

  
  
  memset(&sa, 0, sizeof(sa));
  
  sa.sa_handler = &timer_handler;
  sigaction (SIGVTALRM, &sa, NULL);
  
  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIGVTALRM;
  timer_create(CLOCK_REALTIME, &se, &timerid);
  
  ts.tv_sec = 0;
  ts.tv_nsec = 1000000000/FREQUENCY;
  
  itimer.it_interval = ts;
  itimer.it_value = ts;
  timer_settime(timerid, 0, &itimer, NULL);  
    
  while(1);

  return 0;
}
