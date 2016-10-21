#include "bbcommon.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAX_LINE 50 //maxsize of command or text

//Pprogram that allows interfacing with the LCD.
//While this program is running, text and commands can be written to named pipes
//to manipulate the LCD (command_pipe and text_pipe respectively).
int main() {  
  char text_buf[MAX_LINE];                  
  char com_buf[MAX_LINE];
  
  struct timeval timeout;

  fd_set rdset;
  int text_pipe;
  int command_pipe;

  if ((text_pipe = open("./text_pipe", O_RDONLY | O_NONBLOCK)) == -1) {        //create text pipe
      system("mkfifo text_pipe");
      text_pipe = open("./text_pipe", O_RDONLY | O_NONBLOCK);
  }

  if ((command_pipe = open("./command_pipe", O_RDONLY | O_NONBLOCK)) == -1) {  //create command pipe
      system("mkfifo command_pipe");
      command_pipe = open("./command_pipe", O_RDONLY | O_NONBLOCK);
  }

  close(text_pipe);
  close(command_pipe);

  while (1) { //check named pipes
    text_pipe = open("./text_pipe", O_RDONLY | O_NONBLOCK);
    command_pipe = open("./command_pipe", O_RDONLY | O_NONBLOCK);
    
    FD_ZERO(&rdset);
    FD_SET(text_pipe, &rdset);
    FD_SET(command_pipe, &rdset);

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;
    
    select(FD_SETSIZE, &rdset, NULL, NULL, &timeout);  //find pipes that can be read
    if (FD_ISSET(text_pipe, &rdset)) {         //write text
      read(text_pipe, text_buf, MAX_LINE);
      printf("%s\n", text_buf);

      writeString(text_buf);
      text_buf[0] = '\0';
    }
    if (FD_ISSET(command_pipe, &rdset)) {      //execute command
      read(command_pipe, com_buf, MAX_LINE);
      printf("%s\n", com_buf);

      //check for commands
      if (!strcmp(com_buf, "clear")) {         //clear
	writeCommand(CLEAR_DISP);
      } else if (!strcmp(com_buf, "sdl")) {    //shift display left
	writeCommand(SHIFT_DISP_L);
      } else if (!strcmp(com_buf, "sdr")) {    //shift display right
	writeCommand(SHIFT_DISP_R);
      } else if (!strcmp(com_buf, "scl")) {    //shift cursor left
	writeCommand(SHIFT_CURS_L);
      } else if (!strcmp(com_buf, "scr")) {    //shift cursor right
	writeCommand(SHIFT_CURS_R);
      } else if (!strcmp(com_buf, "bl")) {     //move to bottom line
	writeCommand(0x80 | 0x40);
      } else if (!strcmp(com_buf, "tl")) {     //move to top line
	writeCommand(0x80 | 0x00);
      } else if (!strcmp(com_buf, "b")) {     //move to top line
	writeCommand(SHIFT_CURS_L);
	writeChar(' ');
	writeCommand(SHIFT_CURS_L);
      }
      
      com_buf[0] = '\0';
    }

    close(text_pipe);
    close(command_pipe);
  }
}
