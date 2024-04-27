#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "8puzzle.h"
#include "solve.h"


bool add_new(struct queue *q, struct queue *next_q, int layout, int direction, int deep, char min) {
    assert(q);
    assert(next_q);
    struct board *clone = int_to_3x3(layout);

        if (play_board(clone, direction)) {
            int layout1 = board_to_int(clone);
            if (!(in_queue(q, layout1))) {
                enqueue(next_q, layout1);
                if (deep >= min) {
                    enqueue(q, layout1);
                }
                destroy_board(clone);
                return TRUE;
            }
        }
        destroy_board(clone);
        return FALSE;
}



void permutation_calc(char min, char max, int layout, int cur, int next, char deep,  struct queue *q) {
    assert(q);
    struct queue *cur_q = queue_init();
    struct queue *next_q = queue_init();

    enqueue(q, layout);
    if (min == 0) {
        printw(" 0:     1 |\n");
    }
    while (true) {

        if (deep == max) {
            return;
        }

        for (int i = 0; i < 4; i++) {
            if (add_new(q, next_q, layout, UP + i, deep, min)) {
                next++;
            }
        }

        cur--;
        if (cur == 0) {
            printw("%2d: %5d ", deep + 1, queue_size(next_q));
            for (int i = 0; i < queue_size(next_q); i+=200) {
                printw("|");
            }
            printw("\n");
            refresh();
            queue_destroy(cur_q);
            cur_q = queue_clone(next_q);
            queue_destroy(next_q);
            next_q = queue_init();
            cur = next;
            deep++;
            next = 0;
        }
        layout = dequeue(cur_q);
    }
}

int board_comp(void *board1, void *board2) {
    assert(board1);
    assert(board2);
    struct board *boarda = board1;
    struct board *boardb = board2;
    for (int i = 0; i < get_size(boarda) - 1; i++) {
        if (get_ith_tile(boarda, i) != get_ith_tile(boardb, i)) {
            return 1;
        }
    }
    return 0;
}

int brute_solve(int layout, int cur, int next, char deep,  struct queue *q) {
    assert(q);
    struct queue *cur_q = queue_init();
    struct queue *next_q = queue_init();

    enqueue(q, layout);
    while (true) {
        if (layout == 123456780) {
            queue_destroy(cur_q);
            queue_destroy(next_q);
            return deep;
        }

        for (int i = 0; i < 4; i++) {
            if (add_new(q, next_q, layout, UP + i, deep, 0)) {
                next++;
            }
        }

        cur--;
        if (cur == 0) {
            queue_destroy(cur_q);
            cur_q = queue_clone(next_q);
            queue_destroy(next_q);
            next_q = queue_init();
            cur = next;
            deep++;
            next = 0;
        }
        layout = dequeue(cur_q);
    }
}