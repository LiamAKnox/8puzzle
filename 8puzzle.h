#include <stdbool.h>


extern const int UP;
extern const int DOWN;
extern const int LEFT;
extern const int RIGHT;

struct board;

//board_init(len, wid): creates a board struct for the 8puzzle with the given height and width
//applies shift_cnt random moves to make the board randomized
//max size is 11 x 11
//effects: allocates data [client must call destroy_board]
//requires:  12 > len and wid > 2
struct board *board_init(int len, int wid, int shift_cnt);


//board_clone(board) will create a duplicate of *board and returns a pointer to the dup.
//effects: allocated heap memory [client must call destroy_board]
struct board *board_clone(struct board *board);

//is_solved(board) returns true if the given 8puzzle board is solved, and false otherwise
bool is_solved(struct board *board);

//print_board(board) prints the given board in a visually intuitive way
//effects: writes to window
void print_board(struct board *board);


//destroy_board(board) will free and thus void the given struct board
//effects: invalidates *board
void destroy_board(void *board);

//play_board(board, input) will update the board based on the move attempted by the player
//returns true if it successfully completed a move and false otherwise
//effects: mutates *board
//requires: input must be an arrow key (as integers: 'A', 'B', 'C', 'D')
bool play_board(struct board *board, int input);


//tile_swap(board, idx_shift, null_idx) will swap the boards '0' with the value idx_shift away from '0'
//returns true if it successfully swaps places
//effects: mutates *board
//requires: idx_shift + board->null_idx must still be an index on the board
bool tile_swap(struct board *board, int idx_shift, int null_idx);

//get_count(board) returns how many moves have been played on a board
int get_count(struct board *board);

//get_null_idx(board) returns the index location of a boards '0'
int get_null_idx(struct board *board);

//get_size(board) returns the number of tiles on a board
int get_size(struct board *board);

//get_ith_tile(board, idx) returns the number associated with the idx tile
int get_ith_tile(struct board *board, int idx);

//board_to_int(board) returns the layout of the board as an integer
//requires: board is 3x3 or smaller
int board_to_int(struct board *board);

//int_to_3x3(layout) takes an integer and returns a 3x3 board the boards move count
//will be set to 0
//effects: allocated heap memory [client must call destroy_board]
struct board *int_to_3x3(int layout);
