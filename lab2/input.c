#include "bbcommon.h"

// Interfaces with the terminal to allow the user to write text and commands to the LCD.
// On prompt type text in terminal window to type to LCD, use "/" for commands, and q for quit.
int main() {
  // Turn on the LCD if it isn't already on
  initializeLCD();
  
  int stop = 0;

  while(!stop) {
    char *line = NULL;
    size_t size;
    printf("LCD_DISP (start commands with /, q for quit): "); //prompt user
  
    int len = getline(&line, &size, stdin);

    line[len - 1] = '\0';                  //null terminated string

    if (line[0] == 'q' && len == 2) {      //quit
      stop = 1;
    } else if (line[0] == '/') {           //run command
      int command_pipe = open("command_pipe", O_WRONLY);
      write(command_pipe, line + 1, len - 1);
      close(command_pipe);
    } else {                               //write text
      int text_pipe = open("text_pipe", O_WRONLY);
      write(text_pipe, line, len);
      close(text_pipe);
    }
  }
}
