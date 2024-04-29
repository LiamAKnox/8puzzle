#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "8puzzle.h"
#include "solve.h"

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


int A_star(int *layouts, int *heuristics) {
    assert(layouts);
    assert(heuristics);

    while(true) {
        
    }

}


int heuristic_calc(int layout) {
    int total = 0;

    for (int i = 8; i >= 0; i--) {
        int tile = (layout % 10) - 1;
        total += abs((tile/3) - (i/3)) + abs((tile % 3) - (i % 3));//calculates moves from board tile 'i' to 'tile'
    }

}
/*
void match_sort(int *sort, int *follow, int len) {
    assert(sort);
    assert(follow);
    if (len > 1) {
        int len_left = len / 2;
        int len_right = len - len_left;
        int *sort_left = malloc(len_left * sizeof(int));
        int *follow_left = malloc(len_left * sizeof(struct heuristic));
        int *sort_right = malloc(len_right * sizeof(int));
        int *follow_right = malloc(len_right * sizeof(struct heuristic));

        for (int i = 0; i < len_left; i++) {
            sort_left[i] = sort[i];
            follow_left[i] = sort[i];
        }

        for (int i = 0; i < len_left; i++) {
            sort_right[i] = sort[i + len_left];
            follow_right[i] = sort[i + len_left];
        }

        match_sort(sort_left, follow_left, len_left);
        match_sort(sort_right, follow_right, len_right);



        free(sort_left);
        free(sort_right);
        free(follow_left);
        free(follow_right);



    }


}

void match_merge(int *sort, const int *src1, const struct heuristic *follow1, int len1,
                struct heuristic *follow, const int *src2, const struct heuristic *follow2, int len2) {
    int pos1 = 0;
    int pos2 = 0;
    for (int i = 0; i < len1 + len2; i++) {
        if (pos1 == len1 || (pos2 < len2 && src2[pos2] < src1[pos1])) {
            sort[i] = src2[pos2];
            follow[i] = follow2[pos2];
            pos2++;
        } else {
            sort[i] = src1[pos1];
            follow[i] = follow1[pos1];
            pos1++;
        }
    }
}



*/

//adds layout and new_heuristic to their arrays, and returns the new len
int linked_array_add(int *arr1, int layout, struct heuristic *arr2, struct heuristic new_heuristic, int len, int *max_len) {
    assert(arr1);
    assert(arr2);
    assert(max_len);

    if (len == *max_len) {
        *max_len *=2;
        arr1 = realloc(arr1, max_len);
        arr2 = realloc(arr2, max_len);
    }

    *(arr1 + len) =  layout;
    *(arr2 + len) = new_heuristic;
    return ++len;
}

//removes values at index idx, and shifts all others down, returns new len
int linked_array_remove(int *arr1, int *arr2, int idx, int len) {
    assert(arr1);
    assert(arr2);

    for (int i = idx; i < len - 1; i++) {
        *(arr1 + i) = *(arr1 + i + 1);
        *(arr2 + i) = *(arr2 + i + 1);
    }
    return --len;
}