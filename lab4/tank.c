#include "tank.h"

struct sigaction sa;
struct sigaction quit;

// Information about the GPIOs we're using
struct Gpio GPIOS[] = {
  {27, AIN1_VAL, AIN1_DIR},
  {47, AIN2_VAL, AIN2_DIR},
  {46, BIN1_VAL, BIN1_DIR},
  {65, BIN2_VAL, BIN2_DIR},
  {60, STBY_VAL, STBY_DIR}
};

// Information about the PWMs we're using
struct Pwm PWMS[] = {
  {A_PPATH, A_DPATH, A_RPATH, A_SLOT, START_DUTY},
  {B_PPATH, B_DPATH, B_RPATH, B_SLOT, START_DUTY}
};

int main() {
  //activate GPIOs
  for(int i = 0; i < NUM_DB; i++) {
    activateGPIO(GPIOS[i].num);
    setPin(GPIOS[i].direction_p, "out");
  }

  initializePWMSlots();

  for (int i = 0; i < NUM_PWM; i++) {
    activatePWM(PWMS[i].slot);
  }

  sa.sa_handler = &sighandler;
  sigaction(SIGUSR1, &sa, NULL);

  quit.sa_handler = &exithandler;
  sigaction(SIGINT, &quit, NULL);

  sigaction(SIGTERM, &quit, NULL);

  //system("./adc_listener.exe &");

  drive(100000);

  while(1);
}

//signal handler for tank motors to turn
void sighandler(int signum) {
  printf("Got interrupted\n");
  struct timespec t, t2;

  t.tv_sec = 0;
  t.tv_nsec = 50000000;
  
  while (PWMS[0].duty < PERIOD || PWMS[1].duty < PERIOD) {
    //printf("In while loop\n");
    printf("%d, %d\n", PWMS[0].duty, PWMS[1].duty);
    setDuty(&PWMS[0], PWMS[0].duty + 25000);
    setDuty(&PWMS[1], PWMS[1].duty + 25000);
    nanosleep(&t, &t2);
  }

  drive(-250000);

  t.tv_nsec = 0;
  t.tv_sec = 1;

  nanosleep(&t, &t2);

  turn(100000);
  
  t.tv_sec = 2;

  nanosleep(&t, &t2);

  drive(100000);
}

void exithandler(int signum) {
  isetPin(GPIOS[4].value_p, 0);
  exit(0);
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

void setDuty(struct Pwm *p, int d) {
  if (d > PERIOD) {
    d = PERIOD;
  }

  isetPin(p->duty_p, d);
  p->duty = d;
}

void drive(int d) {
  isetPin(GPIOS[4].value_p, 0);

  isetPin(PWMS[0].run_p, 1);
  isetPin(PWMS[1].run_p, 1);

  if (d < 0) {
    isetPin(GPIOS[0].value_p, 0);
    isetPin(GPIOS[1].value_p, 1);
    isetPin(GPIOS[2].value_p, 0);
    isetPin(GPIOS[3].value_p, 1);
    d = d * -1;
  } else {
    isetPin(GPIOS[0].value_p, 1);
    isetPin(GPIOS[1].value_p, 0);
    isetPin(GPIOS[2].value_p, 1);
    isetPin(GPIOS[3].value_p, 0);
  }

  isetPin(PWMS[0].period_p, PERIOD);
  isetPin(PWMS[1].period_p, PERIOD);

  setDuty(&PWMS[0], d);
  setDuty(&PWMS[1], d);

  isetPin(GPIOS[4].value_p, 1);
}

void turn(int d) {
  isetPin(GPIOS[4].value_p, 0);

  isetPin(PWMS[0].run_p, 1);
  isetPin(PWMS[1].run_p, 1);

  if (d < 0) {
    isetPin(GPIOS[0].value_p, 1);
    isetPin(GPIOS[1].value_p, 0);
    isetPin(GPIOS[2].value_p, 0);
    isetPin(GPIOS[3].value_p, 1);
    d = d * -1;
  } else {
    isetPin(GPIOS[0].value_p, 0);
    isetPin(GPIOS[1].value_p, 1);
    isetPin(GPIOS[2].value_p, 1);
    isetPin(GPIOS[3].value_p, 0);
  }

  isetPin(PWMS[0].period_p, PERIOD);
  isetPin(PWMS[1].period_p, PERIOD);

  setDuty(&PWMS[0], d);
  setDuty(&PWMS[1], d);

  isetPin(GPIOS[4].value_p, 1);
}
