#include <ncurses.h>


void loginPage(void) {
    WINDOW *menu = newwin(6, 40, 0, 0);
    box(menu, 0, 0);

    keypad(menu, true);
    noecho();
    curs_set(0);



}