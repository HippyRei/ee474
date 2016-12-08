#include "adc_listener.h"

//Timer event structures
struct sigaction sa;
struct sigevent se;

//Timer metadata
struct timespec ts; 
struct itimerspec itimer;
timer_t timerid;

//Running total among samples
//int tot1,tot2,tot3,tot4;
int tot [4];
//Sample number
int s;

//Process ID of the tank process
pid_t pid;

//Process ID of the bt_listener
pid_t pid_bt;

int main() {
  printf("THIS IS THE BEGINNING");
  tot[0] = 0;
  tot[1] = 0;
  tot[2] = 0;
  tot[3] = 0;
  s = 0;
 
  printf("IM HERE");
  
  //get PID of tank process
  //from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
  char line[LEN];
  FILE *cmd = popen("pgrep -f tank.exe", "r");

  fgets(line, LEN, cmd);
  pid = strtoul(line, NULL, 10);
  printf("I FOUND TANK");
  
  //get PID of bt_listener
  cmd = popen("pgrep -f bt_listener.exe", "r");
  fgets(line, LEN, cmd);
  pid_bt = strtoul(line, NULL, 10);
  pclose(cmd);
  printf("I FOUND BT_LISTENER");
  
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
  int send_data;
  
  //Increment running total and sample number for each analog input
  tot[0] += read_adc(AIN1); // Front
  tot[1] += read_adc(AIN2); // Right
  tot[2] += read_adc(AIN3); // Left
  tot[3] += read_adc(AIN4); // Rear
  s++;
  //If we've taken FREQUENCY samples, take the average, and interrupt tank.exe if necessary
  if (s == FREQUENCY -1){

    printf("Front: %d ; Right: %d ; Left: %d ; Rear: %d\n", tot[0]/s, tot[1]/s, tot[2]/s, tot[3]/s);
    // if at least one or more running totals reach above threshold
    // we need to change case statements based on what we want

    /*
    //if(tot1/s > = 900){ // only for testing front scenario
    else if(tot1/s >= 900 || tot2/s >= 900 || tot3/s >= 900 || tot4/s >= 900){

      
      printf("You're too close!\n");
      printf("%d\n", pid);
      //send signal
      kill(pid, SIGUSR1);

      // Wait for tank to finish its interupt sequence
      sleep(3);
      
    }*/

    // encode array into 3 byte int (adc val max = 1799/ 29 = 62)
    send_data = (tot[0] / s / 29);
    send_data *= 64;
    send_data += (tot[1] / s / 29);
    send_data *= 64;
    send_data += (tot[2] / s / 29);
    send_data *= 64;
    send_data += (tot[3] / s / 29);

    printf("data2: %d\n",send_data);
    
    
    // Poll results to the bt_listener
    union sigval adc_state;
    adc_state.sival_int = send_data;
    sigqueue(pid_bt, SIGUSR1, adc_state); // send signal to bt_listener

    //Reset Sampling totals
    send_data = 0;
    tot[0] = 0;
    tot[1] = 0;
    tot[2] = 0;
    tot[3] = 0;
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
