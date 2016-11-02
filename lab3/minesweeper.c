#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define BOARD_COL 16
#define BOARD_ROW 2
#define PATH "../../dev/lcd"

void init_board(int board[][BOARD_COL], int num_mines);
void display_board(int board[][BOARD_COL]);
int select(int board[][BOARD_COL], int x, int y);
void uncover_adj(int board[][BOARD_COL], int x, int y);
int num_mine_neighbors(int board[][BOARD_COL], int x, int y);

int main() {
  int board[BOARD_ROW][BOARD_COL];

  int num_mines = 4;
  init_board(board, num_mines);

  display_board(board);
}

void init_board(int board[][BOARD_COL], int num_mines) {
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
      board[y][x] = 1;
      placed++;
    }
  }
}

int select(int board[][BOARD_COL], int x, int y) {
  if (board[y][x] == -1) {
    return -1;
  } else if (board[y][x] == 0) {
    uncover_adj(board, x, y);
  }
}

void uncover_adj(int board[][BOARD_COL], int x, int y) {
  if (x < 0 || x > BOARD_COL - 1 || y < 0 || y > BOARD_ROW) {
    return;
  } else if (board[y][x] != 0) {
    return;
  }

  board[y][x] = 1;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      uncover_adj(board, x + i, y + j);
    }
  }
}

int num_mine_neighbors(int board[][BOARD_COL], int x, int y) { 
  int res = 0;
  
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (board[y + j][x + i] == -1) {
	res++;
      }
    }
  }

  return res;
}

void display_board(int board[][BOARD_COL]) {
  FILE *f = NULL;

  while (f == NULL) {
    f = fopen(PATH, "w");
  }

  fprintf(f, "/clear");
  fflush(f);
  
  for (int j = 0; j < BOARD_ROW; j++) {
    for (int i = 0; i < BOARD_COL; i++) {
      if (board[j][i] == 1) {
	int n_mines = num_mine_neighbors(board, i, j);

	if (n_mines == 0) {
	  fprintf(f, " ");
	  fflush(f);
	} else {
	  fprintf(f, "%d", n_mines);
	  fflush(f);
	}
      } else {
	fprintf(f, "%c" , (char) 0x11111111);
	fflush(f);
      }
    }

    fprintf(f, "/bl");
    fflush(f);
  }

  fclose(f);
}
