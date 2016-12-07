#include "bt_listener.h"

//terminal interface structure.
struct termios ti;
int wait_flag = 1;

//Process ID of the tank_entry process
pid_t pid_tank_entry, pid_tank;

// much of the serial input code taken from http://www.tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
int main() {
  int f, len, command;
  unsigned char buffer[100];
  struct sigaction saio;

  // set PIDs to 0
  pid_tank = 0;
  pid_tank_entry = 0;
  
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

  // Initialize sequence checking connection.
  len = write(f, "hello\n", 6);
  printf("value of len: %d\n", len);

  // get PID of tank_entry process
  //from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
  char line[LEN];
  FILE *cmd = popen("pgrep -f tank_entry.exe", "r");
  fgets(line, LEN, cmd);
  pid_tank_entry = strtoul(line, NULL, 10);

  pclose(cmd);

  // sigval declarations
  union sigval tank_state_on, tank_state_off,tank_drive;
  
  // WE MAY RUN INTO A PROBLEM WITH THE CURRENT STRUCTURE IF WE GET AN INTERRUPT WHILE PROCESSING?
  // IM NOT SURE...
  
  // Continue to loop, only executes a read when interrupted by SIGIO
  while (1) {
    usleep(100000);
    
    if (wait_flag == 0) {
      len = read(f, buffer, 100);
      buffer[len] = 0;
  
      if (len != 0) {
	printf("%s\n", buffer);
      } else {
	printf("nothing read\n");
      }

      command = buffer[0] * 256 + buffer[1];

      if (command == 0xFFFF) {
	//union sigval tank_state_on;
	tank_state_on.sival_int = 1; // "on"
	sigqueue(pid_tank_entry, SIGUSR1, tank_state_on); //send signal to tank

	// we have to wait here somehow, so that tank_entry can create tank -- maybe a signal?
	// i'm implementing with a while loop now.

	while (pid_tank == 0){
	  // get PID of tank process
	  //from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
	  char line[LEN];
	  FILE *cmd = popen("pgrep -f tank.exe", "r");

	  fgets(line, LEN, cmd);
	  pid_tank = strtoul(line, NULL, 10);

	  pclose(cmd);
	}




	
      } else if (command == 0xFF00) {
	//union sigval tank_state_off;
	tank_state_off.sival_int = 0;
	sigqueue(pid_tank_entry, SIGUSR1, tank_state_off); //send signal to tank

	// ensure that new tank pid is obtained on call of 0xFFFF
	pid_tank = 0;

	
      } else {
	//union sigval tank_drive;
	tank_drive.sival_int = command;
	sigqueue(pid_tank, SIGUSR1, tank_drive); //send signal to tank
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

//signal handler for SIGIO
void signal_handler_IO (int status)
{
  printf("received command signal.\n");
  wait_flag = 0;
}
