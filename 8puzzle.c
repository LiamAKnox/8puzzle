#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <ncurses.h>

#include "8puzzle.h"


struct board {
    char *layout;
    char wid;
    char len;
    int moves;
};

struct board *board_init(int len, int wid, int shift_cnt) {
    assert(len > 2 && wid > 2);
    assert(len < 12 && wid < 12);
    struct board *new_board = malloc(sizeof(struct board));
    assert(new_board);
    new_board->moves = 0;
    new_board->wid = wid;
    new_board->len = len;
    new_board->layout = malloc(sizeof(int) * new_board->wid * new_board->len);
    assert(new_board->layout);

    for (int i = 0; i < new_board->wid * new_board->len - 1; i++) {
        *(new_board->layout + i) = i + 1;
    }
    *(new_board->layout + (new_board->len * new_board->wid) - 1) = 0;

    srand(time(NULL));
    for (int i = 0; i < shift_cnt; i++) {
        int scramble = (rand() % 4) + KEY_DOWN;
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
    new_board->wid = board->wid;
    new_board->len = board->len;
    new_board->layout = malloc(sizeof(int) * new_board->wid * new_board->len);
    assert(new_board->layout);
    for (int i = 0; i < new_board->wid * new_board->len; i++) {
        *(new_board->layout + i) = *(board->layout + i);
    }
    return new_board;
}





bool is_solved(struct board *board) {
    assert(board);
    int null_idx = get_null_idx(board);

    if (null_idx == board->wid * board->len - 1) {
        for (int i = 0; i < board->wid * board->len - 2; i++) {
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
            if (*(board->layout + (i * board->wid + j)) == 0) {
                printw("      |");
            } else {
                printw("%4d  |", *(board->layout + (i * board->wid + j)));
            }
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
    int null_idx = get_null_idx(board);

    if (input == KEY_UP && null_idx + board->wid < board->wid * board->len) {
        return tile_swap(board, board->wid, null_idx);
    } else if (input == KEY_DOWN && null_idx - board->wid >= 0) {
        return tile_swap(board, board->wid * -1, null_idx);
    } else if (input == KEY_LEFT && (null_idx + 1) % board->wid != 0) {
        return tile_swap(board, 1, null_idx);
    } else if (input == KEY_RIGHT && null_idx % board->wid != 0) {
        return tile_swap(board, -1, null_idx);
    }
    
    return false;
}

bool tile_swap(struct board *board, int idx_shift, int null_idx) {
    assert(board);
    assert(null_idx != -1);
    board->moves++;
    *(board->layout + null_idx) = *(board->layout + null_idx + idx_shift);
    null_idx += idx_shift;
    *(board->layout + null_idx) = 0;
    return true;
}



int get_count(struct board *board) {
    return board->moves;
}


int get_null_idx(struct board *board) {
    assert(board);
    for (int i = 0; i < board->wid * board->len; i++) {
        if (*(board->layout + i) == 0) {
            return i;
        }
    }
    return -1;
}

int get_size(struct board *board) {
    return board->len * board->wid;
}

int get_ith_tile(struct board *board, int idx) {
    return *(board->layout + idx);
}


int board_to_int(struct board *board) {
    assert(board);
    char *layout = malloc((board->wid * board->len + 1) * sizeof(char));
    assert(layout);
    int num = 0;
    for (int i = 0; i < board->wid * board->len + 1; i++) {
        *(layout + i) = *(board->layout + i) + '0';
    }
    *(layout + board->wid * board->len) = '\0';
    num = atoi(layout);
    free(layout);
    return num;
}

struct board *int_to_3x3(int layout) {
    struct board *board = board_init(3, 3, 0);
    for (int i = 8; i >= 0; i--) {
        *(board->layout + i) = (layout % 10);
        layout /= 10;
    }
    return board;
}
