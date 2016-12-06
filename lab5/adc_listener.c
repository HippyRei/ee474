#include "adc_listener.h"

//Timer event structures
struct sigaction sa;
struct sigevent se;

//Timer metadata
struct timespec ts; 
struct itimerspec itimer;
timer_t timerid;

//Running total among samples
int tot1,tot2,tot3,tot4;
//Sample number
int s;

//Process ID of the tank process
pid_t pid;

int main() {
  tot1 = 0;
  tot2 = 0;
  tot3 = 0;
  tot4 = 0;
  s = 0;
  
  enable_adc();

  //get PID of tank process
  //from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
  char line[LEN];
  FILE *cmd = popen("pgrep -f tank.exe", "r");

  fgets(line, LEN, cmd);
  pid = strtoul(line, NULL, 10);

  pclose(cmd);

  //Set up sigevent for timer 
  memset(&sa, 0, sizeof(sa));
  
  sa.sa_handler = &timer_handler;
  sigaction (SIGVTALRM, &sa, NULL);
  
  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIGVTALRM;
  timer_create(CLOCK_REALTIME, &se, &timerid);
  
  ts.tv_sec = 0;
  ts.tv_nsec = REPORT_T/FREQUENCY;
  
  itimer.it_interval = ts;
  itimer.it_value = ts;
  timer_settime(timerid, 0, &itimer, NULL);  

  //Loop infinitely until killed
  while(1);

  return 0;
}
/*
//Timer interrupt handler (this is with self driving from lab4)
void timer_handler(int signum) {
  //Increment running total and sample number
  tot += read_adc(AIN1);
  s++;

  //If we've taken FREQUENCY samples, take the average, and interrupt tank.exe if necessary
  if (s == FREQUENCY - 1) {
    if (tot / s >= 900) {
      //send signal
      kill(pid, SIGUSR1);

      // Wait for tank to finish its interrupt sequence
      sleep(3);
      
    }
    //Reset the sampling totals
    tot = 0;
    s = 0;
  }
}*/

//Timer interrupt handler (this is for lab5)
void timer_handler(int signum){
  //Increment running total and sample number for each analog input
  tot1 += read_adc(AIN1); // Front
  tot2 += read_adc(AIN2); // Right
  tot3 += read_adc(AIN3); // Left
  tot4 += read_adc(AIN4); // Rear
  s++;
  //If we've taken FREQUENCY samples, take the average, and interrupt tank.exe if necessary
  if (s == FREQUENCY -1){
    
    printf("Front: %d ; Right: %d ; Left: %d ; Rear: %d", tot1, tot2,tot3,tot4);
    // if at least one or more running totals reach above threshold
    // we need to change case statements based on what we want
    if(tot1/s >= 900 || tot2/s >= 900 || tot3/s >= 900 || tot4/s >= 900){

      
      printf("You're too close!\n");
      printf("%d\n", pid);
      //send signal
      kill(pid, SIGUSR1);

      // Wait for tank to finish its interupt sequence
      sleep(3);
      
    }
    //Reset Sampling totals
    tot1 = 0;
    tot2 = 0;
    tot3 = 0;
    tot4 = 0;
    s = 0;
  }
}


//Reads from the adc pin
int read_adc(char *path) {
  //Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (!!access(path, W_OK));
  f = fopen(path, "r");

  //Buffer to store contents of ACD pin
  char buf[101];
  
  int b_len = fread(buf, 1, 100, f);
  buf[100] = 0;
  
  fclose(f);

  return atoi(buf);
}
