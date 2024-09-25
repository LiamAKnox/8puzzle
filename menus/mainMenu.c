#include <ncurses.h>

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
            if(move >= '0' && move <= '5') {//returns proper highlight if its corresponding number is chosen
                return (move - '1');
            }
        
    }

}