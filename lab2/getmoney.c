#include "bbcommon.h"

int main() {
  initializeLCD();

  struct timespec t, t2;

  t.tv_sec = 0;
  t.tv_nsec = 200000000;
  
  while(1) {
    for (int i = 0; i < 16; i++) {
      writeCommand(CLEAR_DISP);
      for (int j = 0; j < i; j++) {
        writeCommand(SHIFT_CURS_R);
      }
      writeChar('$');
      
      writeCommand(0x80 | 0x40);
      writeString("GET MONEY");
      nanosleep(&t, &t2);
    }
  }
}
