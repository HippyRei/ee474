#include "input.h"

// Interfaces with the terminal to allow the user to write text and commands to the LCD.
// On prompt type text in terminal window to type to LCD, use "/" for commands, and q for quit.
int main() {
  
  int stop = 0;

  while(!stop) {
    char *line = NULL;
    size_t size;
    printf("LCD_DISP (start commands with /, q for quit): "); //prompt user
  
    int len = getline(&line, &size, stdin);

    line[len - 1] = '\0';                  //null terminated string

    if (line[0] == 'q' && len == 2) {      //quit
      stop = 1;
    } else  {           //run command
      FILE *f = fopen(PATH, "w");
      fprintf(f, "%s", line);
      fclose(f);
    }
  }
}
