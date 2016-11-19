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

  activateGPIO(49);
  activateGPIO(115);

  setPin(BL1_DIR, "out");
  setPin(BL2_DIR, "out");

  initializePWMSlots();

  for (int i = 0; i < NUM_PWM; i++) {
    activatePWM(PWMS[i].slot);
  }

  activatePWM(BUZZER_SLOT);

  isetPin(BUZZER_RPATH, 0);
  isetPin(BUZZER_PPATH, 1000000);
  isetPin(BUZZER_DPATH, 500000);

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

  isetPin(BL1_VAL, 1);
  isetPin(BL2_VAL, 1);
  isetPin(BUZZER_RPATH, 1);
  drive(-250000);

  t.tv_nsec = 1000000000 - T_BEEPS;
  t.tv_sec = 0;

  nanosleep(&t, &t2);
  isetPin(BUZZER_RPATH, 0);

  t.tv_nsec = T_BEEPS;
  nanosleep(&t, &t2);

  isetPin(BUZZER_RPATH, 1);

  turn(100000);

  t.tv_nsec = 1000000000 - T_BEEPS;

  nanosleep(&t, &t2);

  isetPin(BUZZER_RPATH, 0);

  t.tv_nsec = T_BEEPS;

  nanosleep(&t, &t2);

  isetPin(BUZZER_RPATH, 1);

  t.tv_nsec = 1000000000 - T_BEEPS;

  nanosleep(&t, &t2);

  t.tv_nsec = T_BEEPS;

  isetPin(BUZZER_RPATH, 0);

  nanosleep(&t, &t2);
  isetPin(BL1_VAL, 0);
  isetPin(BL2_VAL, 0);

  drive(100000);
}

void exithandler(int signum) {
  isetPin(GPIOS[4].value_p, 0);
  isetPin(BUZZER_RPATH, 0);
  exit(0);
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
