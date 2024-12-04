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

//#include <mysql.h>;

#include "menu.h"
//#include "statUI.h"
//#include "customizeUI.h"
#include "loginPage.h"
#include "modularUI.h"






//FIXME:    - consider using a pad instead of a window for output to allow smaller terminal sizes to support the formatting
//          -  if not above, at least replace printw with addnstr() with max x, so that formatting isn't ruined
//TODO:     - add mouse events to menu and play 8puzzle

int main(void) {
    int input;
    int len = 3;
    int wid = 3;
    int moves_needed = -1;
    int fd[2];







/*
char **sub_options = calloc(10,sizeof(char *));

    sub_options[0] = "option 1";
    sub_options[1] = "option 2";
    sub_options[2] = "option 3";
    sub_options[3] = "option 4";
    sub_options[4] = "option 5";
    sub_options[5] = "option 6";
    sub_options[6] = "option 7";
    sub_options[7] = "option 8";
    sub_options[8] = "option 9";


    int *return_val = malloc(1 * sizeof(int));
    int *return_val2 = malloc(1 * sizeof(int));
    char *return_string = calloc(16, sizeof(char));
    *return_val = 0;
    *return_val2 = 3;
    



    initscr();
    resizeterm(50, 240);
    keypad(stdscr, TRUE);

    struct MENU *menu = new_menu("NEW MENU");

    add_push_button(menu, "a new push button");
    add_menu_button(menu, "a menu button", sub_options, return_val);
    add_input_spinner(menu, "input spinner", 10, 1, return_val2);
    add_text_input(menu, "text input", 5, 15, true, return_string);


    run_menu(menu);

    endwin();
*/















    //while(true) {
    /////int temp = wgetch();
    //if (temp == KEY_LEFT) {break;}
    //}
    //return 0;
/*
    //start ncurses:
    initscr();
    resizeterm(50, 240);
    keypad(stdscr, TRUE);
    //WINDOW *menu = newwin(5, 40, 0, 0);
    refresh();
    box(menu, 0, 0);
    wrefresh(menu);
    keypad(menu, true);
*/















    initscr();
    resizeterm(50, 240);
    keypad(stdscr, TRUE);




    
    
    while (true) {//loops menu screen
        struct queue *q = queue_init();

        int choice = main_menu();



        clear();
        refresh();
        if (choice == 0) {//1. PLAY 8PUZZLE
            int alg = 0;
            len = 3;
            wid = 3;
            if (!play_puzzle_UI(&len, &wid, &alg)) {continue;}
            if(len != 3 || wid != 3) {alg = 3;}
            struct board *board = board_init(len, wid, 200 * len * wid);
            int layout = board_to_int(board); 
            
            //turn below into a pipe function  
            {
            if (pipe(fd) == -1) {
                printf("an error occurred with opening the pipe");
                return 1;
            }
            int id = fork();
            if (id == 0) {//solves created board
                if (alg == 0) {
                    moves_needed = A_star(layout);
                } else if (alg == 1) {
                    moves_needed = brute_solve(layout, 1, 0, 0, q);
                } else if (alg == 2) {
                    moves_needed = deepening_solve(layout);
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
            

            //possible to turn below into a function?
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
                    break;
                }

            }
            if(input == '\n') {continue;}
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
