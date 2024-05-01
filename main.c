#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <ncurses.h>

#include "8puzzle.h"
#include "queueADT.h"
#include "solve.h"



//currently to run: clang main.c ~/CSWORK/8puzzle-1/8puzzle.c ~/CSWORK/8puzzle-1/queueADT.c ~/CSWORK/8puzzle-1/solve.c ~/CSWORK/8puzzle-1/Astar_list.c -lncurses 
//replace the ~/CSWORK/8puzzle-1/ with the location of the files
//then: ./a.out

//TODO: - implement improved UI (update instructions)
//      - keep track of previous games with a file  
//      - include guards and Makefile


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

//create_menu(choices, num_choices, star_y) creates a simple menu on a window
//with choices as the strings on each line 
//effects: prints to window
//requires: num_choices and start_y > 0, choices must have length of num_choices
int create_menu(char **choices, int num_choices, int start_y) {
    assert(num_choices > 0);
    assert(start_y > 0);
    assert(choices);

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


//play_puzzle_UI(len, wid, alg) creates a menu to let the user decide 
//the length width and algorithm used to calculate efficient moves
//returns true if the user wishes to play the 8puzzle, and false if they want to quit
bool play_puzzle_UI(int *len, int *wid, int *alg) {
    assert(len);
    assert(wid);
    assert(alg);
    char *options[15] = {"", "           +", "", "           -", "",
                            "Breadth First Search", 
                            "A* Search",
                            "Iterative Deepening Search",
                            "Do Not Calculate",
                            "", "", "", "", "", ""};
    char *choices[5] = {"1. HEIGHT: ", "2. WIDTH: ", "3. ALGORITHM: ","4. PLAY", "5. BACK"};
    
    WINDOW *menu = newwin(5 + 2, 50, 0, 0);
    box(menu, 0, 0);

    keypad(menu, true);
    noecho();
    curs_set(0);

    int move;//represents arrow key inputs
    int highlight = 0;//represents which row to highlight
    int side = 0;//represents if you are on left(0) or right(1) side
    int skip = 0;//skips to correct strings in array
    int return_to = 0;//tells right side where to go when returning to left side



    while (true) {
        box(menu, 0, 0);

        if (highlight < 2 && side == 0) {//skip = 0 -> height or width
            skip = 0;
        } else if (highlight == 2 && side == 0) {//skip = 1 -> Algorithm
            skip = 1;
        } else if (side == 0) {//skip = 2 -> Play/Back
            skip = 2;
        }
        for (int i = 0; i < 5; i++) {
            if (i == highlight && side == 0) {//highlights and writes to left side of menu
                wattron(menu, A_REVERSE);
            } 
            mvwprintw(menu, i + 1, 1, "%s", choices[i]);
            wattroff(menu, A_REVERSE);

            //writes to right side of menu
            mvwprintw(menu, i + 1, 20, "%s", options[i + 5 * skip]);
            wattroff(menu, A_REVERSE);
        }

        if (side == 1) {//preps highlighting for printing height/width
            wattron(menu, A_REVERSE);
        }

        if (skip == 0 && ((highlight == 0 && side == 0) || (return_to == 0 && side == 1))) {//if on height, prints height
            mvwprintw(menu, 3, 31, "%d", *len);
        } else if (skip == 0 && ((highlight == 1 && side == 0) || (return_to == 1 && side == 1))) {//if on width, prints width
            mvwprintw(menu, 3, 31, "%d", *wid);
        }

        if (skip == 1) {//ensures proper algorithm is highlighted regardless of side
            wattron(menu, A_REVERSE);
            mvwprintw(menu, *alg + 1, 20, "%s", options[*alg +  5]);
        }
         wattroff(menu, A_REVERSE);

        if (skip == 2) {
            mvwprintw(menu, 1, 20, "%d", *len);
            mvwprintw(menu, 2, 20, "%d", *wid);
            if (*len == 3 && *wid == 3) {
                mvwprintw(menu, 3, 20, "%s", options[*alg +  5]);
            } else {
                mvwprintw(menu, 3, 20, "board must be 3x3 for solving");
            }
        }
        

        move = wgetch(menu);

        wclear(menu);
        wrefresh(menu);
        refresh();
        if (move == KEY_UP && highlight != 0) {//up arrow pressed with an allowed location
            if (side == 0) {//left side
                highlight--;
            } else if (skip == 1) {//on algorithm right side
                (*alg)--;
                highlight--;
            } else if (return_to == 0 && *len < 11) {//(implicit skip == 0, since only skip == 1 or 0 get to side = 1)
                (*len)++;
            } else if (return_to == 1 && *wid < 11) {
                (*wid)++;
            }
        } else if (move == KEY_DOWN && highlight != 4) {
            if (side == 0) {
                highlight++;
            } else if (skip == 1) {//on algorithm right side
                (*alg)++;
                highlight++;
            } else if (return_to == 0 && *len > 3) {//(implicit skip == 0, since only skip == 1 or 0 get to side = 1)
                (*len)--;
            } else if (return_to == 1 && *wid > 3) {
                (*wid)--;
            }
        } else if(move == KEY_RIGHT && side == 0 && skip < 2) {
            side = 1;
            return_to = highlight;
            if (skip == 0) {
                highlight = 2;
            } else if (skip == 1) {
                highlight = *alg;
            }
        } else if (move == KEY_LEFT && side == 1) {
            side = 0;
            highlight = return_to;
        } else if (move == 10) {//enter key
            if (side == 1) {
                side = 0;
                highlight = return_to;
            } else if (highlight == 3) {//play
                if (*len != 3 || *wid != 3) {
                    *alg = 3;
                }
                return true;
            } else if (highlight == 4) {
                return false;
            }
        }
    }
    
}






int main(void) {
    char input;
    int len = 3;
    int wid = 3;
    int moves_needed = -1;
    int fd[2];
    
    //start ncurses:
    initscr();
    WINDOW *menu = newwin(5, 40, 0, 0);
    refresh();
    box(menu, 0, 0);
    wrefresh(menu);
    keypad(menu, true);

    while (true) {//loops menu screen
        struct queue *q = queue_init();

        char *choices[4] = {"1. PLAY 8PUZZLE", "2. CALCULATE SOLVABLE PERMUTATIONS", "3. INSTRUCTIONS", "4. EXIT"};
        int choice = create_menu(choices, 4, 0);
        if (choice == 0) {//1. PLAY 8PUZZLE
            clear();
            int alg = 0;
            len = 3;
            wid = 3;
            if (play_puzzle_UI(&len, &wid, &alg)) {
                struct board *board = board_init(len, wid, 2000);
                int layout = board_to_int(board);   

                if (pipe(fd) == -1) {
                    printf("an error occurred with opening the pipe");
                    return 1;
                }
                int id = fork();
                if (id == 0) {//solves created board
                    if (alg == 0) {
                        moves_needed = brute_solve(layout, 1, 0, 0, q);
                    } else if (alg == 1) {
                        moves_needed = A_star(layout);
                    } else if (alg == 2) {
                        moves_needed = deepening_solve(layout);
                    }
                    close(fd[0]);
                    write(fd[1], &moves_needed, sizeof(int));
                    close(fd[1]);
                    return 0;
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
                
                if (alg != 3) {
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
                clear();
                refresh();
            }

        } else if (choice == 1) {//2. CALCULATE SOLVABLE PERMUTATIONS
            struct board *board = board_init(3, 3, 0);
            int layout = board_to_int(board);

            permutation_calc(0, 31, layout, 1, 0, 0, q);
            printw("total permutations: %d\n", queue_size(q));
            flushinp();
            printw("Press any key to return to menu\n");
            getch();
            queue_destroy(q);//unsure of this line right now
            clear();
            refresh();

        } else if (choice == 2) {//3. INSTRUCTIONS




        } else {//4. EXIT
            endwin();
            queue_destroy(q);
            break;
        }

    }
}