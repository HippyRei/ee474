#include "bt_listener.h"

//terminal interface structure.
struct termios ti;


int main() {
  int f, len;
  unsigned char buffer[100];

  printf("entered loop\n");
  
  enable_UART1();

  printf("entered loop\n");

  usleep(1000);
  //while (!!access(UART1, X_OK));
  f = open(UART1, O_RDWR | O_NOCTTY | O_ASYNC);

  if (f < 0) {
    printf("file failed to open\n");
  }

  memset (&ti, 0, sizeof(ti));

  if (tcgetattr(f, &ti) != 0) {
    printf("tcgetattr failed\n");
  }

  // set file control options
  fcntl(f, F_SETFL, O_ASYNC); // enable asynchronous execution


  cfsetospeed(&ti, BAUDRATE);
  cfsetispeed(&ti, BAUDRATE);

  // OPTIONAL Set termio flags to 0
  /*
  ti.c_cflag = 0;
  ti.c_iflag = 0;
  ti.c_oflag = 0;
  ti.c_lflag = 0;
   */

  ti.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

  // input flag
  //ti.c_iflag &= ~(IXON | IXOFF | IXANY);
  //ti.c_iflag |= (IGNPAR | ICRNL);
  ti.c_iflag = IGNPAR;

  // local flag
  /*
  ti.c_lflag &= ~ICANON;
  ti.c_lflag &= ~(ECHO | ECHOE | ISIG);
  */
  ti.c_lflag = 0;

  // output flag
  //ti.c_oflag &= ~OPOST;
  ti.c_oflag = 0;

  ti.c_cc[VMIN] = 0;
  ti.c_cc[VTIME] = 5;

  tcflush(f, TCIFLUSH);

  // define all the attributes into the buffer
  if (tcsetattr(f, TCSANOW, &ti) != 0) {
    printf("tcsetattr failed\n");
  }
  printf("finished initializing %d\n",f);

  len = write(f, "hello\n", 6);
  printf("value of len: %d\n", len);
  

  while (1) {
    //printf("start of while loop here\n");
    len = read(f, buffer, 100);
    buffer[len] = 0;
    //printf("value of len: %d\n", len);
  
    if (len != 0) {
      printf("%s\n", buffer);
    } else {
      //printf("nothing read\n");
    }
  }
  close(f);
}

void enable_UART1() {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (!!access(DEVPATH, W_OK));
  f =  fopen(DEVPATH, "w");

  fprintf(f, "BB-UART1");
  
  fclose(f);
}
