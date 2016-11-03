#include "getmoney.h"

int main() {

  struct timespec t, t2;

  t.tv_sec = 0;
  t.tv_nsec = 200000000;

  FILE *f = NULL;
  f = fopen(PATH, "w");  

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
