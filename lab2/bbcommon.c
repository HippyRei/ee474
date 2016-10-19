// A collection of beaglebone operations used by multiple programs
#include "bbcommon.h"

// GPIO numbers corresponding to flags
int FLAG_GPIOS[] = {115, 49, 112};
char * FLAG_VALS[] = {E_VAL, RS_VAL, RW_VAL};
char * FLAG_DIRS[] = {E_DIR, RS_DIR, RW_DIR};

// GPIO numbers corresponding to DB0-DB7
int DB_GPIOS[] = {26, 44, 68, 67, 47, 45, 69, 66};
char * DB_VALS[] = {DB7_VAL, DB6_VAL, DB5_VAL, DB4_VAL, DB3_VAL, DB2_VAL, DB1_VAL, DB0_VAL};
char * DB_DIRS[] = {DB7_DIR, DB6_DIR, DB5_DIR, DB4_DIR, DB3_DIR, DB2_DIR, DB1_DIR, DB0_DIR};

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
void setGPIO(char * path, char * flag) {
  // Attempt to open the file; loop until file is found 
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "%s", flag);
  fclose(f);
}

// Write flag to the file corresponding to path (integer version)
void isetGPIO(char * path, int flag) {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (f == NULL) {
    f =  fopen(path, "w");
  }
  
  fprintf(f, "%d", flag);
  fclose(f);
}

// Boot up LCD and get it ready to go
void initializeLCD() {
  struct timespec t, t2;

  // Set up GPIOs on Beaglebone
  for (int i = 0; i < 8; i++) {
    activateGPIO(DB_GPIOS[i]);
    setGPIO(DB_DIRS[i], "out");
  }

  for (int i = 0; i < 3; i++) {
    activateGPIO(FLAG_GPIOS[i]);
    setGPIO(FLAG_DIRS[i], "out");
  }

  // Zero out all values of GPIOs
  setPins(0x0);
  setRS(0);
  setRW(0);

  t.tv_sec = 0;
  t.tv_nsec = 15000000;
  // Wait 15 ms after power is turned on
  nanosleep(&t, &t2);

  // Write first function set command
  setPins(0x30);

  flipE();

  t.tv_nsec = 5000000;
  nanosleep(&t, &t2);

  // Write second function set command
  flipE();

  t.tv_nsec = 100000;
  nanosleep(&t, &t2);
  
  // Write third function set command
  flipE();

  t.tv_nsec = 5000000;
  nanosleep(&t, &t2);

  // Set to 2 lines, 5x10 font
  writeCommand(0x38);

  // Display off
  writeCommand(0x8);

  // Clear display
  writeCommand(0x1);

  // Entry Mode Set
  writeCommand(0x6);

  // Display on
  writeCommand(0xF);
}

// Flips E flag on and off
void flipE() {
  struct timespec t, t2;
  t.tv_sec = 0;
  t.tv_nsec = 500;
  setE(1);
  nanosleep(&t, &t2);
  setE(0);
}

// Set RS flag to flag
void setRS(int flag) {
  isetGPIO(RS_VAL, flag);
}

// Set RW flag to flag
void setRW(int flag) {
  isetGPIO(RW_VAL, flag);
}

// Set E flag to flag
void setE(int flag) {
  isetGPIO(E_VAL, flag);
}

// Set pins to configuration specified by config
void setPins(int config) {
  for (int i = 0; i < NUM_DB; i++) {
    isetGPIO(DB_VALS[NUM_DB - 1 - i], config % 2);
    config /= 2;
  }
}

// Write a command to the GPIO instruction register
void writeCommand(int config) {
  setRS(0);
  setRW(0);
  struct timespec t, t2;
  t.tv_sec = 0;
  t.tv_nsec = 40000;
  setPins(config);
  flipE();
  nanosleep(&t, &t2);
}

// Write a character to the LCD
void writeChar(int config) {
  setRS(1);
  setRW(0);
  struct timespec t, t2;
  t.tv_sec = 0;
  t.tv_nsec = 40000;
  setPins(config);
  flipE();
  nanosleep(&t, &t2);
}

void writeString(char * str) {
  char * cur = str;
  while (*cur) {
    writeChar((int) *cur);
    cur++;
  }
}
