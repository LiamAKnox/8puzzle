#include "8puzzle.h"
#include <ncurses.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

const int UP = 65;
const int DOWN = 66;
const int RIGHT = 67;
const int LEFT = 68;


struct board {
    int *layout;
    int null_idx;
    int wid;
    int len;
    int size;
    int moves;
};



struct board *board_init(int len, int wid, int shift_cnt) {
    assert(len > 1 && wid > 1);
    assert(len < 13 && wid < 13);
    struct board *new_board = malloc(sizeof(struct board));
    assert(new_board);
    new_board->size = len * wid;
    new_board->moves = 0;
    new_board->wid = wid;
    new_board->len = len;
    new_board->null_idx = new_board->size - 1;
    new_board->layout = malloc(sizeof(int) * new_board->size);
    assert(new_board->layout);

    for (int i = 0; i < new_board->size - 1; i++) {
        *(new_board->layout + i) = i + 1;
    }
    srand(time(NULL));
    for (int i = 0; i < shift_cnt; i++) {
        int scramble = (rand() % 4) + UP;
        play_board(new_board, scramble);
    }
    new_board->moves = 0;
    return new_board;
}

struct board *board_clone(struct board *board) {
    assert(board);
    struct board *new_board = malloc(sizeof(struct board));
    assert(new_board);
    new_board->moves = board->moves;
    new_board->size = board->size;
    new_board->wid = board->wid;
    new_board->len = board->len;
    new_board->null_idx = board->null_idx;
    new_board->layout = malloc(sizeof(int) * new_board->size);
    assert(new_board->layout);
    for (int i = 0; i < new_board->size; i++) {
        *(new_board->layout + i) = *(board->layout + i);
    }
    return new_board;
}





bool is_solved(struct board *board) {
    assert(board);
    if (board->null_idx == board->size - 1) {
        for (int i = 0; i < board->size - 2; i++) {
            if (*(board->layout + i) > *(board->layout + i + 1)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void print_board(struct board *board) {
    assert(board);

    for (int j = 0; j < board->wid; j++) {
        printw("-------");
    }
    printw("\n");
    for (int i = 0; i < board->len; i++) {
        printw("|");
        for (int j = 0; j < board->wid; j++) {
            printw("%4d  |", *(board->layout + (i * board->wid + j)));
        }
        printw("\n");
        for (int j = 0; j < board->wid; j++) {
            printw("-------");
        }
        printw("\n");
    }
}




void destroy_board(void *board) {
    
    assert(board);
    struct board *board_access = board;
    free(board_access->layout);
    board_access->layout = NULL;
    free(board_access);
    board = NULL;
}


bool play_board(struct board *board, int input) {
    assert(board);
    if (input == UP && board->null_idx + board->wid < board->size) {
        return tile_swap(board, board->wid);
        
    } else if (input == DOWN && board->null_idx - board->wid >= 0) {
        return tile_swap(board, board->wid * -1);
    } else if (input == LEFT && (board->null_idx + 1) % board->wid != 0) {
        return tile_swap(board, 1);
    } else if (input == RIGHT && board->null_idx % board->wid != 0) {
        return tile_swap(board, -1);
    }
    return false;
}

bool tile_swap(struct board *board, int idx_shift) {
    assert(board);
    board->moves++;
    *(board->layout + board->null_idx) = *(board->layout + board->null_idx + idx_shift);
    board->null_idx += idx_shift;
    *(board->layout + board->null_idx) = 0;
    return true;
}



int get_count(struct board *board) {
    return board->moves;
}

int board_to_int(struct board *board) {
    assert(board);
    char *layout = malloc((board->size) * sizeof(char));
    assert(layout);
    int num = 0;
    for (int i = 0; i < board->size - 1; i++) {
        *(layout + i) = *(board->layout + i) + '0';
    }
    *(layout + board->size - 1) = '\0';
    num = atoi(layout);
    free(layout);
    return num;
}


