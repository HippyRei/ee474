#include <stdio.h>
#include <unistd.h>

#define GPIO67_DIR "../../sys/class/gpio/gpio67/direction"
#define GPIO68_DIR "../../sys/class/gpio/gpio68/direction"
#define GPIO44_DIR "../../sys/class/gpio/gpio44/direction"

#define GPIO67_VAL "../../sys/class/gpio/gpio67/value"
#define GPIO68_VAL "../../sys/class/gpio/gpio68/value"
#define GPIO44_VAL "../../sys/class/gpio/gpio44/value"

#define PWM_SLOT "bone_pwm_P9_14"
#define BUZZER_PPATH "../../sys/devices/ocp.3/pwm_test_P9_14.15/period"
#define BUZZER_DPATH "../../sys/devices/ocp.3/pwm_test_P9_14.15/duty"
#define BUZZER_RPATH "../../sys/devices/ocp.3/pwm_test_P9_14.15/run"

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define PWM_SLOTS_PATH "/sys/devices/bone_capemgr.9/slots"

void setLED(char * path, char * flag);
void toggleIO(char * path);
void setPeriod(int ns);
void activatePWM();
void activateGPIO(int gnum);
void setPWMRun(int flag);

// List of paths to GPIO direction files
char * GPIO_DIR[] = {GPIO67_DIR, GPIO68_DIR, GPIO44_DIR};

// List of paths to GPIO value files
char * GPIO_VAL[] = {GPIO67_VAL, GPIO68_VAL, GPIO44_VAL};

// List of GPIO numbers
int GPIO_NUM[] = {67, 68, 44};

int main() {

  // Activate the GPIOs, switch them to output, and turn them off.
  for (int i = 0; i < 3; i++) {
    activateGPIO(GPIO_NUM[i]);
    toggleIO(GPIO_DIR[i]);
    setLED(GPIO_VAL[i], "0");
  }

  // Activate PWM and set it to run
  activatePWM();
  setPWMRun(1);

  // Loop from 0 to 7 and display the number using LEDs and play tones
  for (int i = 0; i < 8; i++) {
    int cur = i;

    // Turn on and off appropriate LEDs
    for (int j = 2; j >= 0; j--) {
      if (cur % 2 == 1) {
	setLED(GPIO_VAL[j], "1");
      } else {
	setLED(GPIO_VAL[j], "0");
      }
      cur /= 2;
    }

    // Period increments up by 200000
    setPeriod(1300000 + i * 200000);

    // Wait for 1 second
    sleep(1);      
  }

  // Turn off all the LEDs
  for (int i = 0; i < 3; i++) {
    setLED(GPIO_VAL[i], "0");
  }

  // Turn off the buzzer
  setPWMRun(0);
}

// Create configuration folders for PWM EHRPWM1A
void activatePWM() {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(PWM_SLOTS_PATH, "w");
  }

  fprintf(f, "am33xx_pwm");
  fclose(f);

  // Attempt to open the file; loop until file is found
  f = NULL;
  while (f == NULL) {
    f =  fopen(PWM_SLOTS_PATH, "w");
  }

  // Set up configuration folders for EHRPWM1A
  fprintf(f, PWM_SLOT);
  fclose(f);
}

// Set up configuration files for a given GPIO
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

// Turns the GPIO associated with path on or off
void setLED(char * path, char * flag) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, flag);
  fclose(f);
}

// Switch GPIO to output
void toggleIO(char * path) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "out");
  fclose(f);
}

// Set period of the PWM to ns and adjust duty accordingly
void setPeriod(int ns) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f = fopen(BUZZER_PPATH, "w");
  }
  
  fprintf(f,"%d", ns);
  fclose(f);

  // Attempt to open the file; loop until file is found
  f = NULL;
  while (f == NULL) {
    f = fopen(BUZZER_DPATH, "w");
  }

  // Set duty to 1/2 of period
  fprintf(f, "%d", ns / 2);
  fclose(f);
}

// Set the PWM to run
void setPWMRun(int flag) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f = fopen(BUZZER_RPATH, "w");
  }
  
  fprintf(f,"%d", flag);
  fclose(f);
}
