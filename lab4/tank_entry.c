#include "tank_entry.h"

//static int tank_started = 0;

struct sigaction quit;

int main() {
  struct timespec t, t2;

  t.tv_sec = 1;
  
  activateGPIO(SWITCH_GPIO_NUM);

  setPin(SWITCH_DIR, "in");

  int on = 0;

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
  pid_t pid=fork();
  if (pid==0) { /* child process */
    
    static char *argv[]={};
    execv("./tank.exe", argv);
    exit(127); /* only if execv fails */
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

  kill(pid, SIGKILL);
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
