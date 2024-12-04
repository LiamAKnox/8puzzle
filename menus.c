#include "menu.h"
int main_menu() {
    char *choices[5] = {"PLAY 8PUZZLE", "CALCULATE SOLVABLE PERMUTATIONS", "INSTRUCTIONS","STATISTICS", "EXIT"};
    struct MENU *main_menu = new_menu("MAIN MENU");
    for(int i = 0; i < 5; ++i) {
        add_push_button(main_menu, choices[i]);
    }
    return run_menu(main_menu);
}


bool play_puzzle_UI(int *len, int *wid, int *alg) {
    char *alg_options[15] = {   "A* Search",
                            "Breadth First Search", 
                            "Iterative Deepening Search",
                            "Do Not Calculate",};
    char *choices[5] = {"PLAY", "HEIGHT: ", "WIDTH: ", "ALGORITHM: ", "BACK"};
    struct MENU *puzzle_menu = new_menu("8PUZZLE SPECIFICATIONS");

    add_push_button(puzzle_menu, choices[0]);
    add_input_spinner(puzzle_menu, choices[1], 11, 3, len);
    add_input_spinner(puzzle_menu, choices[2], 11, 3, wid);
    add_menu_button(puzzle_menu, choices[3], alg_options, alg);
    add_push_button(puzzle_menu, choices[4]);
    return run_menu(puzzle_menu) == 0;
}


void stat_UI() {
    int len = 3;
    int wid = 3;

    char *choices[3] = {"SHOW STATS", "LENGTH", "WIDTH"};
    struct MENU *stat_menu = new_menu("STAT LOOKUP");

    add_push_button(stat_menu, choices[0]);
    add_input_spinner(stat_menu, choices[1], 11, 3, &len);
    add_input_spinner(stat_menu, choices[2], 11, 3, &wid);
    run_menu(stat_menu);





    {
            //wclear();
            //wrefresh(stat_menu);
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
                
                printw("%2d: moves used: %5d    time: %5d seconds\n", i + 1, moves_needed[count], time_needed[count]);
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