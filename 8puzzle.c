#include "8puzzle.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
            printf("  %d  ", *(board->layout + (i * board->wid + j)));
        }
        printf("\n");
    }
    printf("\n\n");
}




void destroy_board(struct board *board) {
    assert(board);
    free(board->layout);
    board->layout = NULL;
    free(board);
    board = NULL;
}

