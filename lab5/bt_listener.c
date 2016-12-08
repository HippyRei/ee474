#include "bt_listener.h"

//terminal interface structure.
struct termios ti;

// signal handler
struct sigaction saio, adc_receive;

int wait_flag = 1;

int wait_adc_flag = 1;

// Stores adc values in 3 bytes to send to controller.
char adc_string[4] = "000\n";

// Process ID of the tank_entry and tank processes.
pid_t pid_tank_entry, pid_tank;

// 
// much of the serial input code taken from http://www.tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
int main() {
  int f; // serial file descriptor
  int len, command;
  unsigned char buffer[100];

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

  // set up signal handler for adc data retrieval
  adc_receive.sa_flags = SA_SIGINFO;
  adc_receive.sa_sigaction = &signal_handler_ADC;
  sigemptyset(&adc_receive.sa_mask);
  sigaction(SIGUSR1, &adc_receive, NULL);
  printf("got here\n");
  
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
  write(f, "hello\n", 6);

  // get PID of tank_entry process
  //from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
  while (!pid_tank_entry) {
    char line[LEN];
    FILE *cmd = popen("pgrep -f tank_entry.exe", "r");
    fgets(line, LEN, cmd);
    pid_tank_entry = strtoul(line, NULL, 10);
    pclose(cmd);
    
    printf("tank_ENTRY PID is %d\n", pid_tank_entry);

    // check if pid is valid
    int valid = kill(pid_tank_entry, 0);

    if (!!valid) {
      pid_tank_entry = 0;
    }
  }
  
  // Continue to loop, only executes a read when interrupted by SIGIO
  while (1) {
    usleep(100000);
    
    if (!wait_flag) {
      len = read(f, buffer, 100);
      buffer[len] = 0;
  
      if (len != 0) {
	printf("%s\n", buffer);
      } else {
	printf("nothing read\n");
      }

      //send adc information if new info has been received
      if(!wait_adc_flag) {
        write(f, adc_string, 4);
      }

      command = buffer[0] * 256 + buffer[1];

      printf("%X\n", command);

      // Analyze which command to execute.
      if (command == 0xFFFF) {
	union sigval tank_state_on;
	tank_state_on.sival_int = 1;     // on command
	sigqueue(pid_tank_entry, SIGUSR1, tank_state_on); //send signal to tank

	// get PID of tank process each time
	// from http://stackoverflow.com/questions/8166415/how-to-get-the-pid-of-a-process-in-linux-in-c
	while (pid_tank == 0){
	  char line[LEN];
	  FILE *cmd = popen("pgrep -f tank.exe", "r");
	  fgets(line, LEN, cmd);
	  pid_tank = strtoul(line, NULL, 10);
	  pclose(cmd);
	  
	  printf("tank PID is %d\n", pid_tank);

	  // check if pid is valid
	  int valid = kill(pid_tank, 0);

	  if (!!valid) {
	    pid_tank = 0;
	  }
	}
	printf("tank PID is: %d\n", pid_tank);

      } else if (command == 0xFF00) {     // off command
	union sigval tank_state_off;
	tank_state_off.sival_int = 0;
	sigqueue(pid_tank_entry, SIGUSR1, tank_state_off); //send signal to tank

	// ensure that new tank pid is obtained on call of 0xFFFF
	pid_tank = 0;

      } else {                           // drive command
	union sigval tank_drive;
	printf("send drive command\n");
	tank_drive.sival_int = command;
	printf("command: %d\n", command);
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
void signal_handler_IO (int status) {
  printf("received command signal.\n");
  wait_flag = 0;
}

// signal handler of the ADC Data retrieval
void signal_handler_ADC(int signum, siginfo_t * siginfo, void * extra ) {
  int adc_data, front, right, left, rear;
  printf("received adc data.\n");
  adc_data = siginfo->si_value.sival_int;
  int temp1 = adc_data;

  printf("data: %d\n", adc_data);
  
  adc_string[2] = temp1 % 256;
  temp1 /= 256;
  adc_string[1] = temp1 % 256;
  temp1 /= 256;
  adc_string[0] = temp1;

  printf("data: %s\n", adc_string);
	 
  
  rear = (adc_data % 64) * 29;
  adc_data /= 64;
  left = (adc_data % 64) * 29;
  adc_data /= 64;
  right = (adc_data % 64) * 29;
  adc_data /= 64;
  front = adc_data * 29;

  printf("Front: %d ; Right: %d ; Left: %d ; Rear: %d\n", front, right, left, rear);

  // set wait_adc_flag to 0
  wait_adc_flag = 0;

}
