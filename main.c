#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <ncurses.h>

#include "8puzzle.h"
#include "queueADT.h"
#include "solve.h"



//currently to run: clang main.c ~/CSWORK/8puzzle-1/8puzzle.c ~/CSWORK/8puzzle-1/queueADT.c ~/CSWORK/8puzzle-1/solve.c -lncurses 
//replace the ~/CSWORK/8puzzle-1/ with the location of the files
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


int create_menu(char **choices, int num_choices, int start_y) {
    WINDOW *menu = newwin(num_choices + 2, 40, start_y, 0);
    box(menu, 0, 0);

    keypad(menu, true);
    noecho();
    curs_set(0);

    int move;
    int highlight = 0;
    while (true) {
        for (int i = 0; i < num_choices; i++) {
            if (i == highlight) {
                wattron(menu, A_REVERSE);
            }
                mvwprintw(menu, i + 1, 1, "%s", choices[i]);
                wattroff(menu, A_REVERSE);
            }
            move = wgetch(menu);

            if (move == KEY_UP) {
                if (highlight != 0) {
                    highlight--;
                }
            } else if (move == KEY_DOWN) {
                if (highlight != num_choices - 1) {
                    highlight++;
                }
            } 

            if (move == 10) {
                return highlight;
            }
        
    }

}



int main(void) {
    char input;
    int len =3;
    int wid = 3;
    int moves_needed = -1;
    bool calc_moves;
    char size[3] = {'0', '\0', '\0'};
    struct queue *q = queue_init();
    int fd[2];
    /*
struct board *board = board_init(len, wid, 2000);
    int layout1 = board_to_int(board);
    printf("%d", deepening_solve(layout1));
*/


    //start ncurses:
    initscr();
    WINDOW *menu = newwin(5, 40, 0, 0);
    refresh();
    box(menu, 0, 0);
    wrefresh(menu);
    keypad(menu, true);



    
    char *choices[4] = {"1. PLAY 8PUZZLE", "2. CALCULATE SOLVABLE PERMUTATIONS", "3. INSTRUCTIONS", "4. EXIT"};
    int choice = create_menu(choices, 4, 0);

    if (choice == 1) {//2. CALCULATE SOLVABLE PERMUTATIONS
        struct board *board = board_init(3, 3, 0);
        int layout1 = board_to_int(board);
        permutation_calc(0, 31, layout1, 1, 0, 0, q);
        printw("total permutations: %d\n", queue_size(q));
        printw("Press any key to exit\n");
        getch();
        queue_destroy(q);
        endwin();
        return 0;
    } else if (choice == 2) {//3. INSTRUCTIONS
        //do something here (preferably slideable pages of the INSTRUCTIONS.txt file)
    } else if (choice == 3) {//4. EXIT
        endwin();
        queue_destroy(q);
        return 0;
    }
    //1. PLAY 8PUZZLE
    clear();


    echo();
    curs_set(1);
    //accepts a height for the board
    while (true) {
        printw("PLEASE INPUT THE HEIGHT OF THE BOARD (3 - 11):\n");
        getnstr(size, 2);
        len = atoi(size);
        clear();
        if (len > 2 && len < 12) {
            break;
        } else if (*size == 0) {
            endwin();
            exit(0);
        }
        printw("not a valid input: (must be between 3 - 11)\n");
    }



    //accepts a width for the board
    while (true) {
        printw("PLEASE INPUT THE WIDTH OF THE BOARD (3 - 11):\n");
        getnstr(size, 2);
        wid = atoi(size);
        clear();
        if (wid > 2 && wid < 12) {
            clear();
            refresh();
            break;
        } else if (*size == 0) {
            endwin();
            exit(0);
        }
        printw("not a valid input: (must be between 3 - 11)\n");
    }
    noecho();

    clear();
    refresh();

    //initializes the board and prints the starting board
    struct board *board = board_init(len, wid, 2000);


    curs_set(0);
    int layout1 = board_to_int(board);
    int method = 3;
    if (len == 3 && wid == 3) {
        printw("HOW WOULD YOU LIKE TO HAVE THE EFFICIENT MOVES CALCULATED:\n");
        refresh();
        char *methods[4] = {"Breadth First Search", "A* Search", "Iterative Deepening Search", "Do Not Calculate"};
        method = create_menu(methods, 4, 1);
        if (pipe(fd) == -1) {
            printf("an error occured with opening the pipe");
            return 1;
        }

        int id = fork();
        if (id == 0) {
            if (method == 0) {
                    moves_needed = brute_solve(layout1, 1, 0, 0, q);
            } else if (method == 1) {
                //A* method
            } else if (method == 2) {
                moves_needed = deepening_solve(layout1);
            }
            close(fd[0]);
            write(fd[1], &moves_needed, sizeof(int));
            close(fd[1]);


            return 0;
        }
    } 
    clear();
    queue_destroy(q);

    print_board(board);
    refresh();
    

    //repeatedly asks for player's next move until solved or exits('\n')
    ensure_arrow(board);
    while ((input = getch()) != '\n') {
        clear();
        
        play_board(board, input);
        print_board(board);
        refresh();
        if (is_solved(board)) {
            refresh();
            break;
        }
        ensure_arrow(board);
    }
    
    //outputs comparison between user's moves, and efficient move count
    printw("\nmoves used: %d\n", get_count(board));
    
    if (method != 3) {
        close(fd[1]);
        read(fd[0], &moves_needed, sizeof(int));
        close(fd[0]);
        flushinp();
        printw("moves needed: %d\n", moves_needed);
    }
    printw("\n");

    //frees data used by board
    destroy_board(board);

    printw("Press any key to exit\n");
    getch();
    endwin();//ends ncurses
    
}