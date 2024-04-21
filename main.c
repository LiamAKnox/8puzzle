#include <stdio.h>
#include "8puzzle.h"
#include <ncurses.h>

//currently to run: clang main.c ~/CSWORK/8puzzle-1/8puzzle.c -lncurses 
//then: ./a.out





//game suite: currently working on 8puzzle, considering adding other games as practice
//perhaps translate my minesweeper coded in java to consolidate my work
int main(void) {
    int ch;

    initscr();
    struct board *board = board_init(3, 3);
    print_board(board);
    //addstr("add any key to see in the terminal [enter] to exit\n");
    refresh();
    noecho();
    while ((ch = getch()) != '\n') {
        clear();
        //printw("You have pressed %d\n", ch);
        play_board(board, ch);
        print_board(board);
        refresh();
        
    }
    destroy_board(board);

    endwin();

    
}