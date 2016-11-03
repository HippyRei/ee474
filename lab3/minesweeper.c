#include "minesweeper.h"

// Minesweeper game implemented on the 2 by 16 LCD display. The user is initially
// prompted for how many mines to play with (must be between 1 and 31). The user
// can then use the terminal window to move around the board with WASD and select
// a space with e. The user wins if all non-mine spaces are revealed, and loses
// when a mine is selected.

static int c_x = 0;                        // Field for current x position
static int c_y = 0;                        // Field for current y position

static int board[BOARD_ROW][BOARD_COL];    // Field for the board

int main() {
  char buf[10];   // buffer to store user input
  char *p;
  int num_mines;  // number of mines to play with                 

  int stop = 0;   

  // Prompt the user for the number of mines to play with.
  while (!stop) { 
    printf("How many mines would you like to play with? "); 

    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        num_mines = strtol(buf, &p, 10);
    
    	if (!(buf[0] != '\n' && (*p == '\n' || *p == '\0'))) {
    	  printf ("Invalid number entered\n");
    	} else if (num_mines < 1 || num_mines > BOARD_COL * BOARD_ROW - 1) {
    	  printf ("Choose a number between 1 and %d\n", BOARD_COL * BOARD_ROW - 1); 
    	} else {
    	  stop = 1;
    	}
     }
  }
  
  init_board(num_mines);  // initialize the board

  int status = 0;         // status of the game lose: -1  win: 1 continuing: 0

  stop = 0;               

  // Prompt the user for a command to continue the game.
  while(!stop) {
    display_board(status);  // draw the board on the LCD display
    if (status != 0) {      // end game if it has been won or lost
      break;
    }
    
    // open LCD dev file
    FILE *f = NULL;
    while (f == NULL) {
      f = fopen(PATH, "w");
    }
    
    char *line = NULL;
    size_t size;
    printf("WASD, e to select: ");           // prompt user for command
  
    int len = getline(&line, &size, stdin);  // get command from user

    line[len - 1] = '\0';                    // null terminate the string from terminal

    if (line[0] == 'q' && len == 2) {        // quit
      stop = 1;
    } else if (line[0] == 'a') {             // left shift
      if (c_x == 0) {             // edge case
	    c_x = BOARD_COL - 1;
      } else {
	    c_x--;
      }
    } else if (line[0] == 'd' && len == 2) { // right shift
      if (c_x == BOARD_COL - 1) { // edge case
	    c_x = 0;
      } else {
    	c_x++;
      }
    } else if ((line[0] == 's' || line[0] == 'w') && len == 2) {  // up/down
      c_y = !c_y;
    } else if (line[0] == 'e' && len == 2) {  // select
      if (mselect(c_x, c_y) == -1) {
	    status = -1;
      }
    } else {
      printf("Invalid command\n");
    }
    
    // Check whether game has been won or not.
    if (status == 0) {
      status = 1;
      for (int i = 0; i < BOARD_COL; i++) {
        for (int j = 0; j < BOARD_ROW; j++) {
    	  if (board[j][i] == 0) {
    	    status = 0;
    	   }
    	}
      }
    }

    fclose(f);
  }
}

// Initialize the board with the specified number of mines.
void init_board(int num_mines) {
  int placed = 0;

  // Initialize the board to 0s
  for (int i = 0; i < BOARD_COL; i++) {
    for (int j = 0; j < BOARD_ROW; j++) {
      board[j][i] = 0;
    }
  }
  
  srand(time(NULL));

  // Randomly place the mines on the board
  while (placed != num_mines) {
    int x = rand() % BOARD_COL;
    int y = rand() % BOARD_ROW;

    if (board[y][x] == 0) {
      board[y][x] = -1;
      placed++;
    }
  }

  // Print the board for reference.
  for (int j = 0; j < BOARD_ROW; j++) {
    for (int i = 0; i < BOARD_COL; i++) {
      printf("%d ", board[j][i]);
    }
    printf("\n");
  }
  
}

// Select a space on the board to reveal.
int mselect(int x, int y) {
  if (board[y][x] == -1) {        // Selection is a mine
    return -1;
  } else if (board[y][x] == 0) {  // Selection is not a mine
    uncover_adj(x, y);
  }
  return 0;
}

// Recursively unconvers the selected spot and any adjacent spots that are not mines
int uncover_adj(int x, int y) {
  if (x < 0 || x >= BOARD_COL || y < 0 || y >= BOARD_ROW) { // call is outside the board
    return 0;
  } else if (board[y][x] != 0) {               // spot is already revealed
    return 0;
  }

  board[y][x] = 1;                             // reveal current spot

  if (num_mine_neighbors(x, y) == 0) {         // reveal adjacent neighbors
    uncover_adj(x, y + 1);
    uncover_adj(x, y - 1);
    uncover_adj(x + 1, y);
    uncover_adj(x - 1, y);
  }
  return 0;
}

// Counts the number of mines in adjacent spots (including diagonal) to the current spot.
// Returns the number of mines.
int num_mine_neighbors(int x, int y) { 
  int res = 0;       // number of mines

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (x + i >= BOARD_COL || x + i < 0 || y + j >= BOARD_ROW || y + j < 0) {
        continue;
      }
      if (board[y + j][x + i] == -1) {
        res++;
      }
    }   
  }
  return res;
}

// Draws the current status of the board.
void display_board(int status) {
  FILE *f = NULL;

  while (f == NULL) {
    f = fopen(PATH, "w");
  }

  fprintf(f, "/clear");
  fflush(f);

  struct timespec t, t2;
  
  t.tv_sec = 0;
  t.tv_nsec = 5000000;

  nanosleep(&t, &t2);

  if (status == 0) {
    for (int j = 0; j < BOARD_ROW; j++) {
      for (int i = 0; i < BOARD_COL; i++) {
	if (board[j][i] == 1) {
	
	  int n_mines = num_mine_neighbors(i, j);

	  if (n_mines == 0) {
	    fprintf(f, " ");
	    fflush(f);
	  } else {
	    fprintf(f, "%d", n_mines);
	    fflush(f);
	  }
	} else {
	  fprintf(f, "%c", (char) 0xDB);
	  fflush(f);
	}
	/*
	  fprintf(f, "%d", board[j][i] + 1);
	  fflush(f);
	  nanosleep(&t, &t2);
	*/
      }

      fprintf(f, "/bl");
      fflush(f);
    }

    if (!c_y) {
      fprintf(f, "/tl");
      fflush(f);
    }

    for (int i = 0; i < c_x; i++) {
      fprintf(f, "/scr");
      fflush(f);
    }
  } else if (status == -1) {
    fprintf(f, "YOU SUCK");
    fflush(f);
  } else {
    fprintf(f, "YOU WIN!!!!!!");
    fflush(f);
  }

  fclose(f);
}
