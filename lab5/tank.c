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

  //Activate blinker pins
  activateGPIO(49);
  activateGPIO(115);
  setPin(BL1_DIR, "out");
  setPin(BL2_DIR, "out");

  //Activate PWMs
  initializePWMSlots();

  for (int i = 0; i < NUM_PWM; i++) {
    activatePWM(PWMS[i].slot);
  }

  //Set up the buzzer
  activatePWM(BUZZER_SLOT);

  isetPin(BUZZER_RPATH, 0);
  isetPin(BUZZER_PPATH, 1000000);
  isetPin(BUZZER_DPATH, 500000);

  //Set up appropriate interrupt handlers
  sa.sa_sigaction = &sighandler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGUSR1, &sa, NULL);

  quit.sa_handler = &exithandler;
  sigaction(SIGINT, &quit, NULL);

  sigaction(SIGTERM, &quit, NULL);

  //Start driving forward
  drive(100000);

  //Loop infinitely until interrupted
  while(1);
}

//signal handler for tank motors to turn
void sighandler(int signum, siginfo_t * siginfo, void * extra ) {
  struct timespec t, t2;

  t.tv_sec = 0;
  t.tv_nsec = 50000000;

  //Slow down
  while (PWMS[0].duty < PERIOD || PWMS[1].duty < PERIOD) {
    setDuty(&PWMS[0], PWMS[0].duty + 25000);
    setDuty(&PWMS[1], PWMS[1].duty + 25000);
    nanosleep(&t, &t2);
  }

  //Turn blinkers on
  isetPin(BL1_VAL, 1);
  isetPin(BL2_VAL, 1);

  //Turn buzzer on
  isetPin(BUZZER_RPATH, 1);

  //Start backing up
  drive(-250000);

  t.tv_nsec = 1000000000 - T_BEEPS;
  t.tv_sec = 0;

  nanosleep(&t, &t2);
  //Turn buzzer off
  isetPin(BUZZER_RPATH, 0);

  t.tv_nsec = T_BEEPS;
  nanosleep(&t, &t2);

  //Turn buzzer on
  isetPin(BUZZER_RPATH, 1);

  //Start turning left
  turn(100000);

  t.tv_nsec = 1000000000 - T_BEEPS;

  nanosleep(&t, &t2);

  //Turn buzzer off
  isetPin(BUZZER_RPATH, 0);

  t.tv_nsec = T_BEEPS;

  nanosleep(&t, &t2);

  //Turn buzzer on
  isetPin(BUZZER_RPATH, 1);

  t.tv_nsec = 1000000000 - T_BEEPS;

  nanosleep(&t, &t2);

  t.tv_nsec = T_BEEPS;

  //Turn buzzer off
  isetPin(BUZZER_RPATH, 0);

  nanosleep(&t, &t2);
  
  //Turn blinkers off
  isetPin(BL1_VAL, 0);
  isetPin(BL2_VAL, 0);

  //Start driving forward again
  drive(100000);
}

void exithandler(int signum) {
  //Turn everything off
  isetPin(GPIOS[4].value_p, 0);
  isetPin(BUZZER_RPATH, 0);
  isetPin(BL1_VAL, 0);
  isetPin(BL2_VAL, 0);
  exit(0);
}

void setDuty(struct Pwm *p, int d) {
  //Make sure duty isn't greater than period
  if (d > PERIOD) {
    d = PERIOD;
  }

  //Set the duty and update the current duty
  isetPin(p->duty_p, d);
  p->duty = d;
}

//Drive forward if duty is positive or drive backward if duty is negative.
//Absolute duty determines speed of motion
void drive(int d) {
  isetPin(GPIOS[4].value_p, 0);

  isetPin(PWMS[0].run_p, 1);
  isetPin(PWMS[1].run_p, 1);

  if (d < 0) {
    setPins(0x5);
    d = d * -1;
  } else {
    setPins(0xa);
  }

  isetPin(PWMS[0].period_p, PERIOD);
  isetPin(PWMS[1].period_p, PERIOD);

  setDuty(&PWMS[0], d);
  setDuty(&PWMS[1], d);

  isetPin(GPIOS[4].value_p, 1);
}

//Turn left if duty is positive or right if duty is negative.
// Absolute duty determines speed of motion.
void turn(int d) {
  isetPin(GPIOS[4].value_p, 0);

  isetPin(PWMS[0].run_p, 1);
  isetPin(PWMS[1].run_p, 1);

  if (d < 0) {
    setPins(0x9);
    d = d * -1;
  } else {
    setPins(0x6);
  }

  isetPin(PWMS[0].period_p, PERIOD);
  isetPin(PWMS[1].period_p, PERIOD);

  setDuty(&PWMS[0], d);
  setDuty(&PWMS[1], d);

  isetPin(GPIOS[4].value_p, 1);
}

// Set pins to configuration specified by config
void setPins(int config) {
  for (int i = 0; i < NUM_DB - 1; i++) {
    isetPin(GPIOS[NUM_DB - 2 - i].value_p, config % 2);
    config /= 2;
  }
}
