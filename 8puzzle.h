//includes go here:
#include <stdbool.h>

struct board;


//board_init(len, wid): creates a board struct for the 8puzzle with the given height and width
//max size is 12 x 12
//effects: allocates data [client must call destroy_board]
//requires: len and wid > 0
struct board *board_init(int len, int wid);


//is_solved(board) returns true if the given 8puzzle board is solved, and false otherwise
bool is_solved(struct board *board);

//print_board(board) prints the given board in a visually intuitive way
//effects: writes to windown
void print_board(struct board *board);


//destroy_board(board) will free and thus void the given struct board
//effects: invalidates *board
void destroy_board(struct board *board);

//play_board(board, input) will update the board based on the move attempted by the player
//effects: mutates *board
//requires: input must be an arrow key (A, B, C, D)
void play_board(struct board *board, int input);
