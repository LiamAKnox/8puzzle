#include "8puzzle.h"
#include "queueADT.h"

//null_destroy(num) used to supply queueADT's destroy function, does nothing
void null_destroy(void *num); 

int int_board_comp(void *board1, void *board2);


//brute_solve(board) will find the shortest solution to a given board
//returns the solution as a series of 'moves' represented by numbers in an array
//requires: only works for board size: 3x3 or smaller
//effects: allocates memory [no need to free]
int brute_solve(struct board *board, struct queue *q, struct queue *prev);