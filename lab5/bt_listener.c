#include "bt_listener.h"

//terminal interface structure.
struct termios ti;
int wait_flag = 1;

// much of the serial input code taken from http://www.tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
int main() {
  int f, len;
  unsigned char buffer[100];
  struct sigaction saio; 
  
  enable_UART1();

  f = open(UART1, O_RDWR | O_NOCTTY | O_NONBLOCK);

  if (f < 0) {
    printf("file failed to open\n");
  }

  /* install the signal handler before making the device asynchronous */
  saio.sa_handler = signal_handler_IO;
  //saio.sa_mask = 0;
  saio.sa_flags = 0;
  saio.sa_restorer = NULL;
  sigaction(SIGIO,&saio,NULL);

  memset (&ti, 0, sizeof(ti));

  if (tcgetattr(f, &ti) != 0) {
    printf("tcgetattr failed\n");
  }

  // set file control options
  fcntl(f, F_SETOWN, getpid()); // enable this process to receive signal
  fcntl(f, F_SETFL, O_ASYNC); // enable asynchronous execution

  cfsetospeed(&ti, BAUDRATE);
  cfsetispeed(&ti, BAUDRATE);

  // control flags 
  ti.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;

  // input flag
  ti.c_iflag = IGNPAR;

  // local flag
  ti.c_lflag = ICANON;

  // output flag
  ti.c_oflag = 0;

  ti.c_cc[VMIN] = 2;
  ti.c_cc[VTIME] = 0;
  tcflush(f, TCIFLUSH);

  // define all the attributes into the file from termios struct
  if (tcsetattr(f, TCSANOW, &ti) != 0) {
    printf("tcsetattr failed\n");
  }
  printf("finished initializing %d\n",f);

  // Initialize sequence.
  len = write(f, "hello\n", 6);
  printf("value of len: %d\n", len);
  

  while (1) {
    usleep(100000);
    //printf("start of while loop here\n");
    if (wait_flag == 0) {
      len = read(f, buffer, 100);
      buffer[len] = 0;
      //printf("value of len: %d\n", len);
  
      if (len != 0) {
	printf("%s\n", buffer);
      } else {
	//printf("nothing read\n");
      }
      wait_flag = 1;
    }
  }
  close(f);
}

void enable_UART1() {
  // Attempt to open the file; loop until file is found
  FILE *f = NULL;
  while (!!access(DEVPATH, W_OK));
  f =  fopen(DEVPATH, "w");

  fprintf(f, "BB-UART2");
  
  fclose(f);
}

void signal_handler_IO (int status)
{
  printf("received SIGIO signal.\n");
  wait_flag = 0;
}
