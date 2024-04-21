#include "8puzzle.h"
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <assert.h>

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

struct board *board_init(int len, int wid) {
    assert(len > 0 && wid > 0);
    assert(len < 13 && wid < 13);
    struct board *new_board = malloc(sizeof(struct board));
    assert(new_board);
    new_board->moves = 0;
    new_board->size = len * wid;
    new_board->wid = wid;
    new_board->len = len;
    new_board->null_idx = new_board->size - 1;
    new_board->layout = malloc(sizeof(int) * new_board->size);
    assert(new_board->layout);
    for (int i = 0; i < new_board->size - 1; i++) {
        *(new_board->layout + i) = i + 1;
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

    for (int i = 0; i < board->len; i++) {
        for (int j = 0; j < board->wid; j++) {
            printw("  %d  ", *(board->layout + (i * board->wid + j)));
        }
        printw("\n");
    }
    printw("\n\n");
}




void destroy_board(struct board *board) {
    assert(board);
    free(board->layout);
    board->layout = NULL;
    free(board);
    board = NULL;
}


void play_board(struct board *board, int input) {
    assert(board);
    if (input == UP) {
        if (board->null_idx + board->wid < board->size) {
            board->moves++;
            *(board->layout + board->null_idx) = *(board->layout + board->null_idx + board->wid);
            board->null_idx += board->wid;
            *(board->layout + board->null_idx) = 0;
        }
    } else if (input == DOWN) {//maybe make this more efficient, it is literally just the UP, but replaced two '+' with '-'
        if (board->null_idx - board->wid >= 0) {
            board->moves++;
            *(board->layout + board->null_idx) = *(board->layout + board->null_idx - board->wid);
            board->null_idx -= board->wid;
            *(board->layout + board->null_idx) = 0;
        }
    } else if (input == LEFT) {
        if ((board->null_idx + 1) % board->wid != 0) {
            board->moves++;
            *(board->layout + board->null_idx) = *(board->layout + board->null_idx + 1);
            board->null_idx++;
            *(board->layout + board->null_idx) = 0;
        }

    } else if (input == RIGHT) {
        if (board->null_idx % board->wid != 0) {
            board->moves++;
            *(board->layout + board->null_idx) = *(board->layout + board->null_idx - 1);
            board->null_idx--;
            *(board->layout + board->null_idx) = 0;
        }
    }
}