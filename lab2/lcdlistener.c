#include "bbcommon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAX_LINE 16

int main() {
  initializeLCD();
  
  char text_buf[MAX_LINE];
  char com_buf[MAX_LINE];
  
  struct timeval timeout;

  fd_set rdset;
  int text_pipe;
  int command_pipe;

  if ((text_pipe = open("./text_pipe", O_RDONLY | O_NONBLOCK)) == -1) {
      system("mkfifo text_pipe");
      text_pipe = open("./text_pipe", O_RDONLY | O_NONBLOCK);
  }

  if ((command_pipe = open("./command_pipe", O_RDONLY | O_NONBLOCK)) == -1) {
      system("mkfifo command_pipe");
      command_pipe = open("./command_pipe", O_RDONLY | O_NONBLOCK);
  }

  close(text_pipe);
  close(command_pipe);

  while (1) {
    text_pipe = open("./text_pipe", O_RDONLY | O_NONBLOCK);
    command_pipe = open("./command_pipe", O_RDONLY | O_NONBLOCK);
    
    FD_ZERO(&rdset);
    FD_SET(text_pipe, &rdset);
    FD_SET(command_pipe, &rdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    
    select(FD_SETSIZE, &rdset, NULL, NULL, &timeout);
    if (FD_ISSET(text_pipe, &rdset)) {
      read(text_pipe, text_buf, MAX_LINE);
      printf("%s\n", text_buf);

      writeString(text_buf);
      text_buf[0] = '\0';
    }
    if (FD_ISSET(command_pipe, &rdset)) {
      read(command_pipe, com_buf, MAX_LINE);
      printf("%s\n", com_buf);

      if (!strcmp(com_buf, "clear")) {
	writeCommand(CLEAR_DISP);
      } else if (!strcmp(com_buf, "sdl")) {
	writeCommand(SHIFT_DISP_L);
      } else if (!strcmp(com_buf, "sdr")) {
	writeCommand(SHIFT_DISP_R);
      } else if (!strcmp(com_buf, "scl")) {
	writeCommand(SHIFT_CURS_L);
      } else if (!strcmp(com_buf, "scr")) {
	writeCommand(SHIFT_CURS_R);
      } else if (!strcmp(com_buf, "bl")) {
	writeCommand(0x80 | 0x40);
      } else if (!strcmp(com_buf, "tl")) {
	writeCommand(0x80 | 0x00);
      }
      
      com_buf[0] = '\0';
    }

    close(text_pipe);
    close(command_pipe);
  }
}
