#include "ncurses.h"

//play_puzzle_UI(len, wid, alg) creates a menu to let the user decide 
//the length width and algorithm used to calculate efficient moves
//returns true if the user wishes to play the 8puzzle, and false if they want to quit
bool play_puzzle_UI(int *len, int *wid, int *alg) {
    assert(len);
    assert(wid);
    assert(alg);
    char *options[15] = {"", "           +", "", "           -", "",
                            "A* Search",
                            "Breadth First Search", 
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
        } else if (move == 10 || move == '4' || move == '5') {//enter key
            if (side == 1) {
                side = 0;
                highlight = return_to;
            } else if (highlight == 3 || move - '0' == 4) {//play
                if (*len != 3 || *wid != 3) {
                    *alg = 3;
                }
                return true;
            } else if (highlight == 4 || move - '0' == 5) {
                return false;
            } 
        }else if (move >= '0' && move <= '3') {
            highlight = move - '1';
            side = 0;
        } 
    }
    
}