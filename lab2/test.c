#include "bbcommon.h"

int main() {
  //initializeLCD();

  struct timespec t, t2;
  int text_pipe;
  int command_pipe;

  t.tv_sec = 0;
  t.tv_nsec = 200000000;

  //text_pipe = open("text_pipe", O_WRONLY);

  
  while(1) {
    for (int i = 0; i < 16; i++) {
      command_pipe = open("command_pipe", O_WRONLY);
      write(command_pipe, "CLEAR_DISP\0", 11);
      close(command_pipe);
      printf("Got here\n");
      for (int j = 0; j < i; j++) {
	command_pipe = open("command_pipe", O_WRONLY);
	write(command_pipe, "SHIFT_CURS_R\0", 13);
	close(command_pipe);
      }
      text_pipe = open("text_pipe", O_WRONLY);
      write(text_pipe, "$\0", 2);
      close(text_pipe);

      //writeCommand(0x80 | 0x40);
      //write(text_pipe, "GET MONEY", 9);
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
