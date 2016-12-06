#include <unistd.h>
#include <stdio.h>

int main() {
  char buffer[100];

  buffer[0] = '>';
  buffer[1] = 'a';

  int a = buffer[0];
  int b = buffer[1];

  printf("%X\n", a);
  printf("%d\n", a);
  printf("%X\n", b);
  printf("%d\n", b);

  int c = buffer[0]  * 256 + buffer[1];

  printf("%X\n", c);
  printf("%d\n", c);

  if (c == 0x3E61) {
    printf("hi\n");
  }
}
