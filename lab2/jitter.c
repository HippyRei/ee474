#include "jitter.h"

//Test code to flip a GPIO pin on an off in order to demonstrate jitter.
int main() {
  // Initialize GPIO 20
  activateGPIO(20);

  setGPIO(GPIO20_DIR, "out");

  while (1) { //Flip GPIO
    setGPIO(GPIO20_VAL, "1");

    sleep(0.000001);

    setGPIO(GPIO20_VAL, "0");

    sleep(0.000001);
  }
}
