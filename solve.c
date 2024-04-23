#include "solve.h"
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

void null_destroy(void *num) {

}

int int_board_comp(void *board1, void *board2) {
    int *a = board1;
    int *b = board2;
    return *a - *b;
}

int brute_solve(struct board *board, struct queue *q, struct queue *prev){
    assert(q);
    assert(prev);
    assert(board);

    
    refresh();
    if (is_solved(board)) {
        queue_destroy(q, &destroy_board);
        queue_destroy(prev, &null_destroy);
        return get_count(board);
    }

    
    int simple_board = board_to_int(board);
    enqueue(prev, &simple_board);
    
    struct board *clone1 = board_clone(board);
    if (play_board(clone1, UP)) {
        int up_board = board_to_int(clone1);
        if (!(in_queue(prev, &up_board, &int_board_comp))) {
            enqueue(q, clone1);
        }
    } else {
        destroy_board(clone1);
    }

    struct board *clone2 = board_clone(board);
    if (play_board(clone2, DOWN)) {
        int down_board = board_to_int(clone2);
        if (!(in_queue(prev, &down_board, &int_board_comp))) { 
            enqueue(q, clone2);
            
        }
    } else {
        destroy_board(clone2);
    }

    struct board *clone3 = board_clone(board);
    if (play_board(clone3, LEFT)) {
        int left_board = board_to_int(clone3);
        if (!(in_queue(prev, &left_board, &int_board_comp))) {

            enqueue(q, clone3);
            
        }
    } else {
        destroy_board(clone3);
    }

    struct board *clone4 = board_clone(board);
    if (play_board(clone4, RIGHT)) {
        int right_board = board_to_int(clone4);
        if (!(in_queue(prev, &right_board, &int_board_comp))) {
            enqueue(q, clone4);
            
        }
    } else {
        destroy_board(clone4);
    }
    
    return brute_solve(dequeue(q), q, prev);
}