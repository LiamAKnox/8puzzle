#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <unistd.h>
#include <ncurses.h>

#include "8puzzle.h"
#include "queueADT.h"
#include "solve.h"




//ensure_arrow() guarantees that a given input is a special character and the next getch() will 
//give the 'important' character value, or returns a new line
//effects: accepts inputs, may exit the program
int ensure_arrow() {
    char input;
    while ((input = getch()) != 27) {
        if (input == '\n') {
            return 10;
        }
    }
    getch();
    return getch();
}

//create_menu(choices, num_choices, star_y) creates a simple menu on a window
//with choices as the strings on each line 
//effects: prints to window
//requires: num_choices and start_y > 0, choices must have length of num_choices
int create_menu(char **choices, int num_choices, int start_y) {
    assert(num_choices > 0);
    assert(start_y >= 0);
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
    
    WINDOW *menu = newwin(7, 50, 0, 0);
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

//stat_UI() creates a menu for the statistics option. lets users choose height and width
//and then data on that board size is revealed
void stat_UI(void) {
    WINDOW *menu = newwin(5, 30, 0, 0);

    char *choices[3] = {"LENGTH:", "WIDTH:", "SHOW STATS"};
    char *options[6] = {"+", "", "-", " ", " ", " "};

    int move;
    int side = 0;
    int highlight = 0;
    int return_to = 0;
    int len = 3;
    int wid = 3;
    int skip = 0;



    keypad(menu, true);
    noecho();
    curs_set(0);

    while (true) {
        if (highlight < 2) {
            skip = 0;
        } else {
            skip = 1;
        }

        box(menu, 0, 0);
        for (int i = 0; i < 3; i++) {
            if (i == highlight && side == 0) {
                wattron(menu, A_REVERSE);
            }
            mvwprintw(menu, i + 1, 1, "%s", choices[i]);//prints left side of menu
            wattroff(menu, A_REVERSE);

            mvwprintw(menu, i + 1, 15, "%s", options[i + 3 * skip]);//prints right side of menu
        }

        if (side == 1 && skip == 0) {//highlights right side if needed
            wattron(menu, A_REVERSE);
        }

        if (skip == 0 && ((highlight == 0 && side == 0) || (return_to == 0 && side == 1))) {
            mvwprintw(menu, 2, 15, "%d", len);
        } else if (skip == 0){
            mvwprintw(menu, 2, 15, "%d", wid);
        } else {//skip == 1
            mvwprintw(menu, 1, 15, "%d", len);
            mvwprintw(menu, 2, 15, "%d", wid);
        }
        wattroff(menu, A_REVERSE);
        

        move = wgetch(menu);

        if (move == KEY_UP && highlight > 0) {
            if (side == 0) {
                highlight--;
            } else if (return_to == 0 && len < 10) {//inc height 
                len++;
            } else if (wid < 10){//inc width
                wid++;
            }
        } else if (move == KEY_DOWN && highlight < 2) {
            if (side == 0) {
                highlight++;
            } else if (return_to == 0 && len > 3) {
                len--;
            } else if (wid > 3) {
                wid--;
            }
        } else if (move == KEY_RIGHT && side == 0 && skip == 0) {
            side = 1;
            return_to = highlight;
            highlight = 1;
        } else if ((move == KEY_LEFT || move == 10) && side == 1) {
            side = 0;
            highlight = return_to;
        } else if (move == 10 && highlight == 2) {
            wclear(menu);
            wrefresh(menu);
            char file_name[11];
            sprintf(file_name, "%dx%dStats", len, wid);
            FILE *fptr;
            if (!(fptr = fopen(file_name, "r"))) {
                printw("You have no previous games of that size: \n");
                printw("Press any key to exit\n");
                getch();
                return;
            }

            int *moves_needed = calloc(100, sizeof(char *));
            int *time_needed = calloc(100, sizeof(int));


            int count = 0;
            int max = 100;
            while (fscanf(fptr, "%d %d", &time_needed[count], &moves_needed[count]) == 2) {
                count++;
                if (count == max) {
                    max *=2;
                    moves_needed = realloc(moves_needed, max * sizeof(int));
                    time_needed = realloc(time_needed, max * sizeof(int));
                } 
            }   
            count--;
            printw("RECENT GAMES:\n\n");
            int avg_time = 0;
            int avg_moves = 0;
            for (int i = 0; i < count + 1; i++) {
                 
                avg_moves += moves_needed[i];
                avg_time += time_needed[i];
            }
           
            avg_moves /= (count + 1);
            avg_time /= (count + 1);
            printw("Average moves: %d Average time: %d seconds\n\n", avg_moves, avg_time);
            for (int i = 0; i < 10; i++) {
                
                printw("%2d: moves needed: %5d    time: %5d seconds\n", i + 1, moves_needed[count], time_needed[count]);
                count--;
                if (count == -1) {
                    break;
                }
            }
            printw("Press any key to exit\n");
            getch();
            clear();
            refresh();
            return;
        }
    }

}

//FIXME:    - consider using a pad instead of a window for output to allow smaller terminal sizes to support the formatting
//          -  if not above, at least replace printw with addnstr() with max x, so that formatting isn't ruined
//TODO:     - add mouse events to menu and play 8puzzle

int main(void) {
    int input;
    int len = 3;
    int wid = 3;
    int moves_needed = -1;
    int fd[2];
    
    //start ncurses:
    initscr();
    resizeterm(50, 240);
    keypad(stdscr, TRUE);
    WINDOW *menu = newwin(5, 40, 0, 0);
    refresh();
    box(menu, 0, 0);
    wrefresh(menu);
    keypad(menu, true);

    while (true) {//loops menu screen
        struct queue *q = queue_init();

        char *choices[5] = {"1. PLAY 8PUZZLE", "2. CALCULATE SOLVABLE PERMUTATIONS", "3. INSTRUCTIONS","4. STATISTICS", "5. EXIT"};
        int choice = create_menu(choices, 5, 0);
        clear();
        refresh();
        if (choice == 0) {//1. PLAY 8PUZZLE
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
                

                //repeatedly asks for player's next move until solved or exits('\n'
                if ((input = getch()) == '\n') {
                    destroy_board(board);
                    clear();
                    refresh();
                    continue;
                }
                time_t seconds = time(NULL);
                while (input) {
                    clear();
                    
                    play_board(board, input);
                    print_board(board);
                    refresh();
                    if (is_solved(board)) {
                        refresh();
                        break;
                    }
                    if ((input = getch()) == '\n') {
                        destroy_board(board);
                        clear();
                        refresh();
                        continue;
                    }

                }
                seconds = time(NULL) - seconds;

                //outputs comparison between user's moves, and efficient move count
                printw("\nmoves used: %d\n", get_count(board));
                
                if (alg != 3) {
                    close(fd[1]);
                    read(fd[0], &moves_needed, sizeof(int));
                    close(fd[0]);
                    flushinp();
                    printw("moves needed: %d\n", moves_needed);
                }
                printw("Time: %d seconds\n", (int)seconds);
                printw("\n");

                char file_name[11];
                sprintf(file_name, "%dx%dStats", len, wid);
                FILE *fptr;
                fptr = fopen(file_name, "a");
                char data[43];
                sprintf(data, "%d %d\n", (int)seconds, get_count(board));//represents data as time then moves 
                fputs(data, fptr);
                fclose(fptr);



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
            FILE *fptr;
            int page = 0;
            fptr = fopen("INSTRUCTIONS.txt", "r");
            char page1[3000] = {""};
            char page2[3000] = {""};
            char page3[3000] = {""};
            char page4[3000] = {""};
            char line[200];

            for (int i = 0; i < 33; i++) {
                fgets(line, 200, fptr);
                strcat(page1, line);
            }
            for (int i = 0; i < 30; i++) {
                if (fgets(line, 200, fptr)) {
                    strcat(page2, line);
                }
            }
            for (int i = 0; i < 31; i++) {
                if (fgets(line, 200, fptr)) {
                    strcat(page3, line);
                }
            }
            for (int i = 0; i < 31; i++) {
                if (fgets(line, 200, fptr)) {
                    strcat(page4, line);
                }
            }

            fclose(fptr);

            char *pages[4] = {page1, page2, page3, page4};
            while (true) {
                int input;
                printw("%s", pages[page]);
                WINDOW *pg_num = newwin(1, 5, 36, 44);
                keypad(pg_num, true);
                mvwprintw(pg_num, 0, 0, "< ");
                mvwprintw(pg_num, 0, 2, "%d", page + 1);
                mvwprintw(pg_num, 0, 3, " >");
                refresh();
                wrefresh(pg_num);
                input = wgetch(pg_num);
                clear();

                if (input == KEY_RIGHT && page < 3) {
                    page++;
                } else if (input == KEY_LEFT && page > 0) {
                    page--;
                } else if (input == 10) {
                    clear();
                    refresh();
                    break;
                }
            }

        } else if (choice == 3) {//4. STATISTICS
            stat_UI();

        }else {//5. EXIT
            endwin();
            queue_destroy(q);
            break;
        }

    }
}