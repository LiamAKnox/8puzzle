#include <stdio.h>
#include "8puzzle.h"
#include <ncurses.h>

//currently to run: clang main.c ~/CSWORK/8puzzle-1/8puzzle.c -lncurses 

//then: ./a.out





//game suite: currently working on 8puzzle, considering adding other games as practice
int main(void) {
    int ch;

    initscr();
    struct board *board = board_init(3, 3);
    print_board(board);
    refresh();
    noecho();
    while ((ch = getch()) != '\n') {
        clear();
        play_board(board, ch);
        print_board(board);
        refresh();
        
    }
    destroy_board(board);

    endwin();
}