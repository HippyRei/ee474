#include "jitter.h"

int main() {
  
  // Initialize GPIO 20
  activateGPIO(20);

  setGPIO(GPIO20_DIR, "out");

  while (1) {
    setGPIO(GPIO20_VAL, "1");

    sleep(0.000001);

    setGPIO(GPIO20_VAL, "0");

    sleep(0.000001);
  }
}
