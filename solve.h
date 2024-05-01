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
//returns the number of moves required
//requires: board must be 3 x 3
//effects: allocates memory [no need to free]
int brute_solve(int layout, int cur, int next, char deep,  struct queue *q);

int deepening_solve(int layout);

bool DFS_maxed(int layout, char deep, char max_deep, char prev);

/*
A*:
list based search algorithm where list is sorted based on heuristics(distance away from solved + moves already taken)
loops looking at the first in the sorted list applying the all new board making moves and adding these new board into the list 

the heuristic:
    moves is obvious
    distance from solved:
        -simply the number of tiles not in the right place
        -or maybe add the distance of each tile from its correct placement (ignoring '0's placement)


*/
int A_star(int layout);

/*
//match_sort(sort, follow) sorts both *follow and *sort in ascending order of the elements in *sort
//ex. sort = {2, 3, 1}    follow = {12, 16, 41}   -> sort = {1, 2, 3}   follow = {41, 12, 16}
//requires: *sort and *follow must have the same length
void match_sort(int sort, struct heuristic *follow);

void match_merge(int *sort, const int *src1, const struct heuristic *follow1, int len1,
                struct heuristic *follow, const int *src2, const struct heuristic *follow2, int len2);


*/

int heuristic_calc(int layout);
