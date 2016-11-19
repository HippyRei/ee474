#include "tank_entry.h"

int main() {
  pid_t pid1, pid2;

  //Initialize PWM slots and ADC slots
  initializePWMSlots();
  activatePWM(A_SLOT);
  activatePWM(B_SLOT);
  enable_adc();
  activatePWM(BUZZER_SLOT);

  //Activate switch
  activateGPIO(SWITCH_GPIO_NUM);
  setPin(SWITCH_DIR, "in");

  //Turn on green LED to let user know the program's ready
  activateGPIO(LED_GPIO_NUM);
  setPin(LED_DIR, "out");
  isetPin(LED_VAL, 1);

  //Loop infinitely
  while (1) {
    struct timespec t, t2;

    t.tv_sec = 1;

    int on = 0;

    while (!on) {
      FILE *f = NULL;
      while (!!access(SWITCH_VAL, R_OK));
      f = fopen(SWITCH_VAL, "r");

      // Buffer to store contents of GPIO
      char buf[10];
  
      int b_len = fread(buf, 1, 10, f);
      buf[9] = 0;
  
      fclose(f);

      on = atoi(buf);

      nanosleep(&t, &t2);
    }
    
    //Create a child to run the tank program.
    pid1 = fork();
    if (pid1 == 0) { //Tank process
      static char *argv[] = {};
      execv("/root/lab4/tank.exe", argv);
      exit(127);
    } else {
      //Create a child to run the acd_listener program
      pid2 = fork();

      if (pid2 == 0) { //adc_listener process
	static char *argv[]={};
	execv("/root/lab4/adc_listener.exe", argv);
	exit(127);
      }
    }

    while (on) {
      FILE *f = NULL;
      while (!!access(SWITCH_VAL, R_OK));
      f =  fopen(SWITCH_VAL, "r");

      // Buffer to store contents of GPIO
      char buf[10];
  
      int b_len = fread(buf, 1, 10, f);
      buf[9] = 0;
  
      fclose(f);

      on = atoi(buf);

      nanosleep(&t, &t2);
    }

    //Kill the 2 children
    kill(pid1, SIGTERM);
    kill(pid2, SIGKILL);

    int status;

    //Wait on the 2 children
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
  }
  return 0;
}
