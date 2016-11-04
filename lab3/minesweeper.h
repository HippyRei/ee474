#ifndef _MINESWEEPER_H_
#define _MINESWEEPER_H_

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

#endif
