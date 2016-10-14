// A collection of beaglebone operations used by multiple programs
#include "bbcommon.h"

int GPIO_NUMS[] = {49, 117, 26, 44, 68, 67, 47, 45, 69, 66, 115};

char * GPIO_VALS[] = {RS_VAL, RW_VAL, DB7_VAL, DB6_VAL, DB5_VAL, DB4_VAL, DB3_VAL, DB2_VAL, DB1_VAL, DB0_VAL, E_VAL};

char * GPIO_DIRS[] = {RS_DIR, RW_DIR, DB7_DIR, DB6_DIR, DB5_DIR, DB4_DIR, DB3_DIR, DB2_DIR, DB1_DIR, DB0_DIR, E_DIR};

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

void setGPIO(char * path, char * flag) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "%s", flag);
  fclose(f);
}

void isetGPIO(char * path, int flag) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "%d", flag);
  fclose(f);
}

void initializeLCD() {
  for (int i = 0; i < 11; i++) {
    activateGPIO(GPIO_NUMS[i]);
    setGPIO(GPIO_DIRS[i], "out");
  }

  int config[] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 0};
  setPins(config, 10);

  sleep(0.005);

  setPins(config, 10);

  sleep(0.0002);

  setPins(config, 10);

  sleep(1);

  int config2[] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1};

  setPins(config2, 10);
}

void setPins(int * config, int len) {
  for (int i = 0; i < len; i++) {
    isetGPIO(GPIO_VALS[i], config[i]);
  }
}
