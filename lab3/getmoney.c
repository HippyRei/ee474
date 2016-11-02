// For getline() and nanosleep()
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define PATH "../../dev/lcd"

int main() {

  struct timespec t, t2;

  t.tv_sec = 0;
  t.tv_nsec = 200000000;

  FILE *f = NULL;
  f = fopen(PATH, "w");

  /*
  fprintf(f, "yo");
  fclose(f);

  nanosleep(&t, &t2);

  f = fopen(PATH, "w");
  fprintf(f, "/clear");
  fclose(f);

  nanosleep(&t, &t2);

  f = fopen(PATH, "w");
  fprintf(f, "yo, again");
  fclose(f);
  */
  

  while(1) {
    for (int i = 0; i < 16; i++) {      
      //f = fopen(PATH, "w");
      fprintf(f, "/clear");
      fflush(f);
      for (int j = 0; j < i; j++) {	
        fprintf(f, "/scr");
	
	fflush(f);
      }
      
      fprintf(f, "$");
      fflush(f);

      fprintf(f, "/bl");
      fflush(f);
      fprintf(f, "GET MONEY");
      fflush(f);
      nanosleep(&t, &t2);
    }
  }
}
