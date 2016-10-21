#include "bbcommon.h"

int main() {
  initializeLCD();

  struct timespec t, t2;
  int text_pipe;
  int command_pipe;

  t.tv_sec = 0;
  t.tv_nsec = 200000000;

  struct timespec loopt;
  loopt.tv_sec = 1;
  loopt.tv_nsec = 0;

  //text_pipe = open("text_pipe", O_WRONLY);

  
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
  

  /*
  text_pipe = open("text_pipe", O_WRONLY);
  write(text_pipe, "Hello\0", 6);
  close(text_pipe);
  
  command_pipe = open("command_pipe", O_WRONLY);
  write(command_pipe, "SHIFT_CURS_R\0", 13);
  close(command_pipe);
  */
}
