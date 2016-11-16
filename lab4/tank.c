#include "tank.h"

struct sigaction sa;

// GPIO numbers corresponding to DB0-DB7
int DB_GPIOS[] = {27, 47, 46, 65, 60};
char * DB_VALS[] = {AIN1_VAL, AIN2_VAL, BIN1_VAL, BIN2_VAL, STBY_VAL};
char * DB_DIRS[] = {AIN1_DIR, AIN2_DIR, BIN1_DIR, BIN2_DIR, STBY_DIR};


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

// Write flag to the file corresponding to path (integer version)
void isetPin(char * path, int flag) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "%d", flag);
  fclose(f);
}

// Set pins to configuration specified by config
void setPins(int config) {
  for (int i = 0; i < NUM_DB; i++) {
    isetPin(DB_VALS[NUM_DB - 1 - i], config % 2);
    config /= 2;
  }
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


//signal handler for tank motors to turn
void sighandler(int signum) {
  //do something
  //back up and turn
}



int main() {

  //activate GPIOs
  for(int i = 0; i < 5; i++) {
    activateGPIO(DB_GPIOS[i]);
    setPin(DB_DIRS[i], "out");
  }

  

  sa.sa_handler = &sighandler;
  sigaction(SIGUSR1, &sa, NULL);

  while(1) {
    //drive forward
  }

}

