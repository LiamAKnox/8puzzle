#ifndef A31E2EFC_C5C3_4935_ADA1_315B7D3A5B90
#define A31E2EFC_C5C3_4935_ADA1_315B7D3A5B90
#include "queueADT.h"
#include <stdbool.h>

struct heuristic;


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


//brute_solve(layout, cur, next, deep, q) will find the shortest solution to a given board
//returns the number of moves required, using a BFS 
//requires: board must be 3 x 3
//effects: allocates memory [no need to free]
int brute_solve(int layout, int cur, int next, char deep,  struct queue *q);


//deepening_solve(layout) returns the fewest number of moves to solve a 3x3 board
//implements a DFS that has a maxed depth, if the DFS fails to find a solution,
//the max depth will increase and the DFS repeats until a solution is found
int deepening_solve(int layout);

//DFS_maxed(layout, deep, max_deep, prev) returns true if a solution to a 3x3 board 
//is possible in max_deep many moves or less using a DFS
bool DFS_maxed(int layout, char deep, char max_deep, char prev);


//A_star(layout) takes a 3x3 board and solves for the efficient
//A* search uses a list of nodes, each with information on the number of moves applied to a node's board
//and a heuristic value of how close a board is to being solved
//it iteratively finds the node with the smallest sum of the above two values, and creates and adds all 
//children nodes to the list until it finds a solution then returns the number of moves required to get there
//requires: layout must represent a 3x3 board
int A_star(int layout);

//heuristic_calc(layout) finds the heuristic value of a board used in the A_star search
//it is the sum of all tiles' manhattan distances from their solved position
int heuristic_calc(int layout);


#endif /* A31E2EFC_C5C3_4935_ADA1_315B7D3A5B90 */
