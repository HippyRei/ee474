#include "bbcommon.h"

int main() {
  //initializeLCD();

  struct timespec t, t2;
  int text_pipe;
  int command_pipe;

  t.tv_sec = 0;
  t.tv_nsec = 200000000;

  //text_pipe = open("text_pipe", O_WRONLY);

  /*
  while(1) {
    for (int i = 0; i < 16; i++) {
      write(command_pipe, "CLEAR_DISP", 10);
      for (int j = 0; j < i; j++) {
	write(command_pipe, "SHIFT_CURS_R", 12);
      }
      write(text_pipe, "$", 1);

      //writeCommand(0x80 | 0x40);
      //write(text_pipe, "GET MONEY", 9);
      nanosleep(&t, &t2);
    }
  }
  */

  command_pipe = open("command_pipe", O_WRONLY);
  write(command_pipe, "SHIFT_CURS_R", 12);
  close(command_pipe);
}
