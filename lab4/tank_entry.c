#include "tank_entry.h"

//static int tank_started = 0;

struct sigaction quit;

//static int tank_run = 0;

int main() {
  pid_t pid1, pid2;

  initializePWMSlots();
  activatePWM(A_SLOT);
  activatePWM(B_SLOT);

  //sleep(1);

  enable_adc();

  activateGPIO(LED_GPIO_NUM);

  setPin(LED_DIR, "out");
  setPin(LED_VAL, "1");
  
  while (1) {
    struct timespec t, t2;

    t.tv_sec = 1;
  
    activateGPIO(SWITCH_GPIO_NUM);

    setPin(SWITCH_DIR, "in");

    int on = 0;

    system("echo got > /root/didit");

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

    printf("%d, %d\n", pid1, pid2);

    kill(pid1, SIGTERM);
    kill(pid2, SIGKILL);

    int status;

    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
  }
  return 0;
}

// Write flag to the file corresponding to path
void setPin(char * path, char * flag) {
  // Attempt to open the file; loop until file is found 
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "%s", flag);
  fclose(f);
}

// Activate the GPIO corresponding to gnum
void activateGPIO(int gnum) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(GPIO_EXPORT_PATH, "w");
  }

  // Create configuration files for GPIO gnum
  fprintf(f, "%d", gnum);
  fclose(f);
}

void enable_adc() {
   // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (!access(ACD_SLOTS_PATH, W_OK));
  f =  fopen(ACD_SLOTS_PATH, "w");
  system("echo \"got here\" > /root/didit");

  fprintf(f, "cape-bone-iio");
  /*
  FILE *test = NULL;
  test = fopen(AIN1, "r");

  while (test == NULL) {
    sleep(1);
    // Enable acd ports so we can read from them
    fprintf(f, "cape-bone-iio");
    test = fopen(AIN1, "r");
  }

  fclose(test);
  */
  
  fclose(f);
}

void initializePWMSlots() {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(PWM_SLOTS_PATH, "w");
  }

  fprintf(f, "am33xx_pwm");
  fclose(f);
}

// Create configuration folders for PWM EHRPWM1A
void activatePWM(char * pwm) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(PWM_SLOTS_PATH, "w");
  }

  // Set up configuration folders for EHRPWM1A
  fprintf(f, "%s", pwm);
  fclose(f);
}
