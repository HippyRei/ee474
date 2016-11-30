#include "bt_listener.h"

//terminal interface structure.
struct termios ti;


int main() {
  enable_UART1();

  cfsetospeed(&ti, 

}

void enable_UART1() {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (!!access(DEVPATH, W_OK));
  f =  fopen(DEVPATH, "w");

  fprintf(f, "BB-UART1");
  
  fclose(f);
}
