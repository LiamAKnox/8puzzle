#include "queueADT.h"
#include <stdbool.h>



//add_new(q, next_q, layout, direction, deep, min) attempts to apply the 'direction' move onto layout
//if successful, enqueues the new layout onto *next_q, and if deep >= min enqueues the same board onto *q
//returns true if the move was successful and false otherwise
//effects: may affect *q and *next_q
bool add_new(struct queue *q, struct queue *next_q, int layout, int direction, int deep, char min);

//permutation_calc(min, max, layout, cur, next, deep, q) calculates all of the board 
//permutations that take between min and max number of moves to solve, and stores them in q
//requires: min >= 0, max <= 31, min <= max
//effects: prints to window
void permutation_calc(char min, char max, int layout, int cur, int next, char deep,  struct queue *q);

//board_comp(board1, board2) compares two board structures, and returns 0 if they are the 
//same permutation and 1 otherwise
//requires: boards must have the same dimensions
int board_comp(void *board1, void *board2);


//brute_solve(layout, cur, next, deep, q) will find the shortest solution to a given board
//returns the number of moves required
//requires: board must be 3 x 3
//effects: allocates memory [no need to free]
int brute_solve(int layout, int cur, int next, char deep,  struct queue *q);