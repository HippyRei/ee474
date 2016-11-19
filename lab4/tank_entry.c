#include "tank_entry.h"

int main() {
  pid_t pid1, pid2;

  initializePWMSlots();
  activatePWM(A_SLOT);
  activatePWM(B_SLOT);

  enable_adc();

  activatePWM(BUZZER_SLOT);

  activateGPIO(LED_GPIO_NUM);

  setPin(LED_DIR, "out");
  isetPin(LED_VAL, 1);
  
  while (1) {
    struct timespec t, t2;

    t.tv_sec = 1;
  
    activateGPIO(SWITCH_GPIO_NUM);

    setPin(SWITCH_DIR, "in");

    int on = 0;

    while (!on) {
      FILE *f = NULL;
      while (f == NULL) {
	f =  fopen(SWITCH_VAL, "r");
      }

      // Buffer to store contents of GPIO
      char buf[10];
  
      int b_len = fread(buf, 1, 10, f);
      buf[9] = 0;
  
      fclose(f);

      on = atoi(buf);

      nanosleep(&t, &t2);
    }
    
    /*Spawn a child to run the program.*/
    pid1 = fork();
    if (pid1 == 0) { /* child process */
    
      static char *argv[]={};
      execv("/root/lab4/tank.exe", argv);
      exit(127); /* only if execv fails */
    } else {
      pid2 = fork();

      if (pid2 == 0) {
      
	static char *argv[]={};
	execv("/root/lab4/adc_listener.exe", argv);
	exit(127); /* only if execv fails */
      }
    }

    while (on) {
      FILE *f = NULL;
      while (f == NULL) {
	f =  fopen(SWITCH_VAL, "r");
      }

      // Buffer to store contents of GPIO
      char buf[10];
  
      int b_len = fread(buf, 1, 10, f);
      buf[9] = 0;
  
      fclose(f);

      on = atoi(buf);

      nanosleep(&t, &t2);
    }

    kill(pid1, SIGTERM);
    kill(pid2, SIGKILL);

    int status;

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
  }
  return 0;
}
