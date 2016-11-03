#define _GNU_SOURCE

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define BOARD_COL 16
#define BOARD_ROW 2
#define PATH "../../dev/lcd"

void init_board(int num_mines);
void display_board(int);
int mselect(int x, int y);
int uncover_adj(int x, int y);
int num_mine_neighbors(int x, int y);
static int c_x = 0;
static int c_y = 0;

static int board[BOARD_ROW][BOARD_COL];

int main() {
  int num_mines = 4;
  init_board(num_mines);

  int stop = 0;
  int status = 0;

  while(!stop) {
    display_board(status);
    if (status != 0) {
      break;
    }
    
    FILE *f = NULL;
    while (f == NULL) {
      f = fopen(PATH, "w");
    }
    
    char *line = NULL;
    size_t size;
    printf("WASD, e to select: "); //prompt user
  
    int len = getline(&line, &size, stdin);

    line[len - 1] = '\0';                  //null terminated string

    if (line[0] == 'q' && len == 2) {     //quit
      stop = 1;
    } else if (line[0] == 'a') {          
      if (c_x == 0) {
	c_x = BOARD_COL - 1;
	c_y = !c_y;
      } else {
	c_x--;
      }
    } else if (line[0] == 'd') {
      if (c_x == BOARD_COL - 1) {
	c_x = 0;
	c_y = !c_y;
      } else {
	c_x++;
      }
    } else if (line[0] == 's' || line[0] == 'w') {
      c_y = !c_y;
    } else if (line[0] == 'e') {
      if (mselect(c_x, c_y) == -1) {
	status = -1;
      }
    } else {
      printf("Invalid command\n");
    }

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

void init_board(int num_mines) {
  int placed = 0;
  
  for (int i = 0; i < BOARD_COL; i++) {
    for (int j = 0; j < BOARD_ROW; j++) {
      board[j][i] = 0;
    }
  }

  srand(time(NULL));

  while (placed != num_mines) {
    int x = rand() % BOARD_COL;
    int y = rand() % BOARD_ROW;

    if (board[y][x] == 0) {
      board[y][x] = -1;
      placed++;
    }
  }

  for (int j = 0; j < BOARD_ROW; j++) {
    for (int i = 0; i < BOARD_COL; i++) {
      printf("%d ", board[j][i]);
    }
    printf("\n");
  }
  
}

int mselect(int x, int y) {
  if (board[y][x] == -1) {
    return -1;
  } else if (board[y][x] == 0) {
    uncover_adj(x, y);
  }
  return 0;
}

int uncover_adj(int x, int y) {
  if (x < 0 || x >= BOARD_COL || y < 0 || y >= BOARD_ROW) {
    return 0;
  } else if (board[y][x] != 0) {
    return 0;
  }

  board[y][x] = 1;

  /*
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      uncover_adj(x + i, y + j);
    }
  }
  */

  if (num_mine_neighbors(x, y) == 0) {
    uncover_adj(x, y + 1);
    uncover_adj(x, y - 1);
    uncover_adj(x + 1, y);
    uncover_adj(x - 1, y);
  }
  
  return 0;
}

int num_mine_neighbors(int x, int y) { 
  int res = 0;
  
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
