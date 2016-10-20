#include "bbcommon.h"

int main() {
  int stop = 0;

  while(!stop) {
    char *line = NULL;
    size_t size;
    printf("LCD_DISP (start commands with /, q for quit): ");
  
    int len = getline(&line, &size, stdin);

    line[len - 1] = '\0';

    if (line[0] == 'q' && len == 2) {
      stop = 1;
    } else if (line[0] == '/') {
      int command_pipe = open("command_pipe", O_WRONLY);
      write(command_pipe, line + 1, len - 1);
      close(command_pipe);
    } else {
      int text_pipe = open("text_pipe", O_WRONLY);
      write(text_pipe, line, len);
      close(text_pipe);
    }
  }
}
