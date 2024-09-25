#include <ncurses.h>


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
            float avg_time = 0;
            int avg_moves = 0;
            for (int i = 0; i < count + 1; i++) {
                 
                avg_moves += moves_needed[i];
                avg_time += time_needed[i];
            }
           
            avg_moves /= (count + 1);
            avg_time /= (count + 1);
            printw("Average moves: %d Average time: %.2f seconds\n\n", avg_moves, avg_time);
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