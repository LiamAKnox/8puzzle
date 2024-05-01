#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "8puzzle.h"
#include "solve.h"
#include "Astar_list.h"

struct heuristic {
    int moves;
    int distance;
};


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

int deepening_solve(int layout) {
    for (int i = 0; i < 31; i++) {
        if (DFS_maxed(layout, 0, i, 0)) {
            return i;
        }
    }
    return 31;

}

bool DFS_maxed(int layout, char deep, char max_deep, char prev) {
    if (layout == 123456780) {
        return true;
    }
    if (deep == max_deep) {
        return false;
    }

    for (int i = 0; i < 4; i++) {
        struct board* board = int_to_3x3(layout);
        if (UP + i == prev) {
             destroy_board(board);
             continue;
        }
        if (play_board(board, UP + i)) {
            int reverse;
            if (i == 0 || i == 2) {
                reverse = 1;
            } else {
                reverse = -1;
            }
            if (DFS_maxed(board_to_int(board), deep + 1, max_deep, UP + i + reverse)) {
                destroy_board(board);
                return true;
            }
            destroy_board(board);
            
        }
    }
    return false;
}

//things to fix: you forgot to have the 'closed list' and fix the stupid free problem (though maybe [who knows why but] closed list will fix it)
int A_star(int layout) {

    if (layout == 123456780) {
        return 0;
    }
    struct Astar_node *origin = Astar_node_init(layout, 0, 0);
    struct Astar_list *list = Astar_list_init();
    struct Astar_list *closed = Astar_list_init();
    list_add(list, origin);//adds starting node to the list

    while(true) {//repeats until solved

        struct Astar_node *cur_node = list_remove_smallest(list);//takes and removes node closest to completion
        list_add(closed, cur_node);//adds this node to the closed list
        for (int i = 0; i < 4; i++) {//this should add all new nodes
            struct board *cur_board = int_to_3x3(get_layout(cur_node));//turns node into a board
            play_board(cur_board, UP + i);//attempts a move on the board
            int new_layout = board_to_int(cur_board);
            destroy_board(cur_board);
            if (new_layout == 123456780) {//if new board solved return moves required
                int moves = get_moves(cur_node) + 1;
                destroy_list(closed);
                destroy_list(list);
                return moves;
            }
            //turn board into a node 
            struct Astar_node *new_node = Astar_node_init(new_layout, get_moves(cur_node) + 1, heuristic_calc(new_layout));

            
            //if new node has already been checked, dont add it
            if (in_list(closed, new_node) ) {
                free(new_node);
            } else if (!list_add(list, new_node)) {//try adding it
                free(new_node);
            }
        }

        
        //repeat this process
    }

}


int heuristic_calc(int layout) {
    int distance = 0;

    for (int i = 8; i >= 0; i--) {
        int tile = (layout % 10) - 1;
        distance += abs((tile/3) - (i/3)) + abs((tile % 3) - (i % 3));//calculates moves from board tile 'i' to 'tile'
        layout /= 10;
    }
    return distance;
}




