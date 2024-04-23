#include <stdio.h>
#include "8puzzle.h"
#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>
#include "queueADT.h"
#include "solve.h"
#include <sys/types.h>
#include <sys/resource.h>

//getting to 8puzzle.c is different depending on where it is stored

//currently to run: clang main.c ~/CSWORK/8puzzle-1/8puzzle.c ~/CSWORK/8puzzle-1/queueADT.c ~/CSWORK/8puzzle-1/solve.c -lncurses 
//then: ./a.out



//ensure_arrow(board) guarantees that a given input is a special character and the next getch() will 
//give the 'important' character value
//effects: accepts inputs, may exit the program
void ensure_arrow(struct board *board) {
    assert(board);
    char input;
    while ((input = getch()) != 27) {
        if (input == '\n') {
            destroy_board(board);
            endwin();
            exit(0);
        }
    }
    getch();
}


int main(void) {
    char input;
    int len =3;
    int wid = 3;
    int shift_cnt = 2000;
    int moves_needed;
    bool calc_moves;
    char size[3] = {'0', '\0', '\0'};
    struct queue *q = queue_init(sizeof(struct board *));
    struct queue *prev = queue_init(sizeof(int *));

    //allowing more memory for use in brute_solve
    struct rlimit the_limit = { RLIM_INFINITY, RLIM_INFINITY};
    if (-1 == setrlimit(RLIMIT_AS, &the_limit)) {
    perror("setrlimit failed");
    }

    //start ncurses:
    initscr();
    
    //accepts a height for the board
    while (true) {
        printw("PLEASE INPUT THE HEIGHT OF THE BOARD (2 - 12):\n");
        refresh();
        getnstr(size, 2);
        len = atoi(size);
        clear();
        if (len > 1 && len < 13) {
            break;
        }
        printw("not a valid input: (must be between 2 - 12)\n");
    }

    //accepts a width for the board
    while (true) {
        printw("PLEASE INPUT THE WIDTH OF THE BOARD (2 - 12):\n");
        refresh();
        getnstr(size, 2);
        wid = atoi(size);
        clear();
        if (wid > 1 && wid < 13) {
            break;
        }
        printw("not a valid input: (must be between 2 - 12)\n");
    }

    while ( wid <= 3 && len <= 3) {
        printw("WOULD YOU LIKE TO HAVE THE EFFICIENT MOVES CALCULATED(Y/N):\n");
        printw("Note: please enter ctrl + c if the program halts inputs for more than a few seconds\n");
        refresh();
        getnstr(size, 1);
        clear();
        if (*size == 'Y') {
            calc_moves = TRUE;
            shift_cnt = 60;
            break;
        } else if (*size == 'N') {
            calc_moves = FALSE;
            break;
        }
        printw("PLEASE ENTER Y or N (capitalised)\n");
    }




    //initializes the board and prints the starting board
    struct board *board = board_init(len, wid, shift_cnt);
    printf("%d\n", board_to_int(board));
    print_board(board);
    refresh();
    noecho();

    if (calc_moves) {
        moves_needed = brute_solve(board_clone(board), q, prev);
    }

    //repeatedly asks for player's next move until solved or exits('\n')
    ensure_arrow(board);
    
    while ((input = getch()) != '\n' && !is_solved(board)) {
        clear();
        play_board(board, input);
        print_board(board);
        refresh();
        ensure_arrow(board);
    }
    

    
    //outputs comparison between user's moves, and efficient move count
    printw("\nmoves used: %d\n", get_count(board));
    if (calc_moves) {
        printw("moves needed: %d\n", moves_needed);
    }
    printw("\n");
    refresh();

    //frees data used by board
    destroy_board(board);

    printw("Press any key to exit\n");
    getch();
    endwin();//ends ncurses
}