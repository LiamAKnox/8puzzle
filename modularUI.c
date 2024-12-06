#include "modularUI.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

const int option_y_offset = 3;//represents different between the y value of an option and the option's number(ex. 1. option_Name is printed on the 4th(1 + 3) line)
const int sub_option_x_offset = 10; //subject to change
int sub_option_x = -1;
int num_of_options = -1;
int box_x_size = -1;
int box_y_size = -1;

enum MENU_TYPE {
    PUSH_BUTTON, 
    MENU_BUTTON,
    INPUT_SPINNER,
    TEXT_INPUT,
};


struct MENU_ITEM {
    char *name;
    char **sub_options;
    int num_of_sub_options;
    enum MENU_TYPE menu_item_type;
    int min;
    int max;
    bool password;
    void *return_val;
};



struct MENU {
    WINDOW *window;
    char *title;
    int num_of_options;
    int max_sub_options;
    int longest_option;
    int longest_sub_option;
    int sub_option_x;
    struct MENU_ITEM **menu_items;
};


struct MENU *new_menu(char *title) {
    WINDOW *window = newwin(10, 100, 0, 0);
    struct MENU *menu = malloc(sizeof(struct MENU ));
    if(menu == NULL) {
        exit(1);
    }
    menu->window = window;
    menu->title = title;

    menu->num_of_options = 0;
    menu->max_sub_options = 0;
    menu->longest_option = 0;
    menu->longest_sub_option = 0;
    menu->sub_option_x = 0;
    menu->menu_items = NULL;
    
    return menu;
}

int run_menu(struct MENU *menu) {
    
    num_of_options = menu->num_of_options;
    sub_option_x = menu->longest_option + sub_option_x_offset;
    box_y_size = (menu->num_of_options > menu->max_sub_options ? menu-> num_of_options : menu->max_sub_options) + option_y_offset + 1;
    box_x_size = menu->longest_option + menu->longest_sub_option + sub_option_x_offset + 2;
    
    char**options = malloc(sizeof(char *) * menu->num_of_options);
    for(int i = 0; i < menu->num_of_options; ++i) {
        options[i] = menu->menu_items[i]->name;
    }
    build_UI(menu->window, menu->title, options, menu->num_of_options, menu->max_sub_options, menu->longest_option, menu->longest_sub_option);

    int cur_option = 0;

    while(true) {
        struct MENU_ITEM *cur_item = menu->menu_items[cur_option];
        int returned_input = -1;

        if(cur_item->menu_item_type == PUSH_BUTTON || cur_item->menu_item_type == TEXT_INPUT) {
            for(int i = 0; i < menu->num_of_options; ++i) {
                if(menu->menu_items[i]->menu_item_type == PUSH_BUTTON) {continue;}
                else if (menu->menu_items[i]->menu_item_type == MENU_BUTTON) {
                    mvwprintw(menu->window, i + option_y_offset,menu->sub_option_x, "%s", menu->menu_items[i]->sub_options[*(int *)menu->menu_items[i]->return_val]);
                } else if (menu->menu_items[i]->menu_item_type == INPUT_SPINNER) {
                    mvwprintw(menu->window, i + option_y_offset,menu->sub_option_x, "%d", *(int *)menu->menu_items[i]->return_val);
                } else if (menu->menu_items[i]->menu_item_type == TEXT_INPUT) {
                    mvwprintw(menu->window, i + option_y_offset,menu->sub_option_x, "%s", (char *)menu->menu_items[i]->return_val);
                }
            }
        }

        if(cur_item->menu_item_type == PUSH_BUTTON) {
            returned_input = regular_clickable(menu->window, options[cur_option], cur_option);
        } else if (cur_item->menu_item_type == MENU_BUTTON) {
            returned_input = options_menu_item(menu->window, options[cur_option], cur_option, cur_item->sub_options, cur_item->num_of_sub_options, cur_item->return_val);
        } else if (cur_item->menu_item_type == INPUT_SPINNER) {
            returned_input = number_menu_item(menu->window, options[cur_option], cur_option, cur_item->min, cur_item->max, cur_item->min, cur_item->return_val);
        } else if (menu->menu_items[cur_option]->menu_item_type == TEXT_INPUT) {
            returned_input = string_input_menu_item(menu->window, options[cur_option], cur_option, cur_item->max,cur_item->return_val, cur_item->password);
        }
        clear_right_side(menu);

        if(returned_input == KEY_UP && cur_option - 1 >= 0) {
            --cur_option;
        } else if (returned_input == KEY_DOWN && cur_option + 1 < menu->num_of_options) {
            ++cur_option;
        } else if(returned_input > 0 && returned_input <= menu->num_of_options) {
            cur_option = returned_input - 1;
        } else if(returned_input == 10 && cur_item->menu_item_type == PUSH_BUTTON) {
            werase(menu->window);
            wrefresh(menu->window);
            free(options);
            return cur_option;
        }
    }
}

void destroy_menu_item(struct MENU_ITEM *menu_item){
    free(menu_item);
}


void destroy_menu(struct MENU *menu) {
    for(int i = 0; i < num_of_options; ++i) {
        destroy_menu_item(menu->menu_items[i]);
    }
    free(menu->menu_items);
    delwin(menu->window);
    free(menu);
    menu = NULL;
}

//push_button, menu_button, spinner_button, text_input
#pragma region ADD_BUTTONS
void add_push_button(struct MENU *menu, char *name) {
    struct MENU_ITEM *menu_item = malloc(sizeof(struct MENU_ITEM));
    if(menu_item == NULL) {
        exit(1);
    }
    menu_item->name = name;
    menu_item->menu_item_type = PUSH_BUTTON;
    menu_item->sub_options = NULL;

    menu->num_of_options++;


    menu->longest_option = (menu->longest_option > strlen(name))? menu->longest_option: strlen(name);
    menu->sub_option_x = menu->longest_option + sub_option_x_offset;
    menu->menu_items = realloc(menu->menu_items, menu->num_of_options * sizeof(struct MENU_ITEM *));
    if(menu->menu_items == NULL) {
        exit(1);
    }
    menu->menu_items[menu->num_of_options - 1] = menu_item;

}


void add_menu_button(struct MENU *menu, char *name, char **sub_options, int *return_destination) {
    struct MENU_ITEM *menu_item = malloc(sizeof(struct MENU_ITEM));
    if(menu_item == NULL) {
        exit(1);
    }
    menu_item->name = name;
    menu_item->menu_item_type = MENU_BUTTON;
    menu_item->sub_options = sub_options;
    menu_item->return_val = return_destination;

    menu->num_of_options++;

    menu->longest_option = menu->longest_option > strlen(name)? menu->longest_option: strlen(name);
    menu->sub_option_x = menu->longest_option + sub_option_x_offset;

    int count = 0;
    while(sub_options[count] != NULL) {
        menu->longest_sub_option = menu->longest_sub_option > strlen(sub_options[count])? menu->longest_sub_option: strlen(sub_options[count]);
        ++count;
    }
    menu_item->num_of_sub_options = count;
    menu->max_sub_options = menu->max_sub_options > count? menu->max_sub_options: count;


    menu->menu_items = realloc(menu->menu_items, menu->num_of_options * sizeof(struct MENU_ITEM *));
    if(menu->menu_items == NULL) {
        exit(1);
    }
    menu->menu_items[menu->num_of_options - 1] = menu_item;
}



void add_input_spinner(struct MENU *menu, char *name, int max_val, int min_val, int *return_destination) {
    struct MENU_ITEM *menu_item = malloc(sizeof(struct MENU_ITEM));
    if(menu_item == NULL) {
        exit(1);
    }
    menu_item->name = name;
    menu_item->menu_item_type = INPUT_SPINNER;
    menu_item->min = min_val;
    menu_item->max = max_val;
    menu_item->return_val = return_destination;

    menu->num_of_options++;

    menu->longest_option = menu->longest_option > strlen(name)? menu->longest_option: strlen(name);
    menu->sub_option_x = menu->longest_option + sub_option_x_offset;
    menu->longest_sub_option = menu->longest_sub_option > 11? menu->longest_sub_option: 11;
    menu->max_sub_options = menu->max_sub_options > 3? menu->max_sub_options: 3;

    menu->menu_items = realloc(menu->menu_items, menu->num_of_options * sizeof(struct MENU_ITEM *));
    if(menu->menu_items == NULL) {
        exit(1);
    }
    menu->menu_items[menu->num_of_options - 1] = menu_item;
}



void add_text_input(struct MENU *menu, char *name, int shortest, int longest, bool password, char *return_destination) {
    struct MENU_ITEM *menu_item = malloc(sizeof(struct MENU_ITEM));
    if(menu_item == NULL) {
        exit(1);
    }
    menu_item->name = name;
    menu_item->menu_item_type = TEXT_INPUT;
    menu_item->min = shortest;
    menu_item->max = longest;
    menu_item->return_val = return_destination;
    
    menu->num_of_options++;

    menu->longest_option = menu->longest_option > strlen(name)? menu->longest_option: strlen(name);
    menu->sub_option_x = menu->longest_option + sub_option_x_offset;
    menu->longest_sub_option = menu->longest_sub_option > longest? menu->longest_sub_option: longest;
    menu->max_sub_options = menu->max_sub_options > 1? menu->max_sub_options: 1;

    menu->menu_items = realloc(menu->menu_items, menu->num_of_options * sizeof(struct MENU_ITEM *));
    if(menu->menu_items == NULL) {
        exit(1);
    }
    menu->menu_items[menu->num_of_options - 1] = menu_item;
}
#pragma endregion



#pragma region BUTTON_LOGIC


int regular_clickable(WINDOW *menu, char *option, int option_num) {
    wattron(menu, A_REVERSE);
    mvwprintw(menu, option_num + option_y_offset, 1, "%d. %s", option_num + 1, option);
    wattroff(menu, A_REVERSE);
    wrefresh(menu);
    int move = -1;
    while(true) {
        move = wgetch(menu);
        if(move == 10) {
            return move;
        } else if(move > '0' && move <= num_of_options + '0') {
            mvwprintw(menu, option_num + option_y_offset, 1, "%d. %s", option_num + 1, option);
            return move - '0';
        } else if(move == KEY_UP || move == KEY_DOWN || move == KEY_RIGHT) {
            mvwprintw(menu, option_num + option_y_offset, 1, "%d. %s", option_num + 1, option);
            return move;
        }
    }
}


void print_sub_option(WINDOW *menu, char *sub_option, int sub_option_num) {
    mvwprintw(menu, sub_option_num + option_y_offset, 1 + sub_option_x, "%s", sub_option);
    wrefresh(menu);
}





void build_UI(WINDOW *menu, char *title, char **options, int options_count, int max_sub_count, int longest_option, int longest_sub_option) {
    num_of_options = options_count;
    

    ///WINDOW *menu = newwin(box_y_size, box_x_size, 0, 0);
    int y = 4 + ((max_sub_count > options_count)? max_sub_count: options_count);
    wresize(menu, y, sub_option_x + longest_sub_option + 4);
    
    box(menu, 0, 0);

    keypad(menu, true);
    noecho();
    curs_set(0);

    mvwprintw(menu, 1, 1, "%s", title);
    for (int i = 0; i < options_count; i++) {
        mvwprintw(menu, i + option_y_offset, 1, "%d. %s", (i + 1), options[i]);//prints left side of menu
    }

    wrefresh(menu);
}

void clear_right_side(struct MENU *menu) {
    for(int i = 2; i < box_y_size; ++i) {
        wmove(menu->window, i, menu->longest_option + sub_option_x_offset - 1);
        wclrtoeol(menu->window);
    }
    box(menu->window, 0, 0);
    wrefresh(menu->window);
}



int options_menu_item(WINDOW *menu, char *option, int option_num, char **sub_options, int sub_options_count, int *chosen_sub) {
    for(int i = 0; i < sub_options_count; ++i) {
        if(i == *chosen_sub) {wattron(menu, A_REVERSE);}
        print_sub_option(menu, sub_options[i], i);
        wattroff(menu, A_REVERSE);
    }
    int move = regular_clickable(menu, option, option_num);
    if(move != KEY_RIGHT) {
        return move;
    }

    while(move != KEY_LEFT) {
        wattron(menu, A_REVERSE);
        print_sub_option(menu, sub_options[*chosen_sub], *chosen_sub);
        wattroff(menu, A_REVERSE);
        move = wgetch(menu);
        


        if(move == KEY_UP && *chosen_sub > 0) {
            --*chosen_sub;
        } else if (move == KEY_DOWN && *chosen_sub < sub_options_count - 1) {
            ++*chosen_sub;
        }

        for(int i = 0; i < sub_options_count; ++i) {
            if(i == *chosen_sub) {
                wattron(menu, A_REVERSE);
            }
            print_sub_option(menu, sub_options[i], i);
            wattroff(menu, A_REVERSE);
        }
    }
    return move;
}


int number_menu_item(WINDOW *menu, char *option, int option_num, int standard, int max, int min, int *current_num) {
    int middle_layer = ceil((num_of_options + 6) / 2);
    int x_pos = floor(((box_x_size - sub_option_x)/2));
    
    

    mvwprintw(menu, middle_layer - 1, sub_option_x + x_pos, "+");
    mvwprintw(menu, middle_layer + 1, sub_option_x + x_pos, "-");
    mvwprintw(menu, middle_layer, sub_option_x + x_pos, "%d", *current_num);

    int move = regular_clickable(menu, option, option_num);
    if(move != KEY_RIGHT) {
        return move;
    }

    while(move != KEY_LEFT) {
        wmove(menu, middle_layer, sub_option_x);
        wclrtoeol(menu);
        box(menu, 0, 0);

        wattron(menu, A_REVERSE);
        mvwprintw(menu, middle_layer, sub_option_x + x_pos, "%d", *current_num);
        wattroff(menu, A_REVERSE);

        wrefresh(menu);

        move = wgetch(menu);

        if(move == KEY_UP && *current_num < max) {
            ++*current_num;
        } else if (move == KEY_DOWN && *current_num > min) {
            --*current_num;
        }
    }
    return move;
}

void remove_at(char *string, int n) {
    assert(string[n]);
    for(int i = n; i < strlen(string); ++i) {
        string[i] = string[i + 1];
    }
}

void add_at(char *string, char insert, int n) {
    for(int i = strlen(string) + 1; i > n; --i) {
        string[i] = string[i - 1];
    }
    string[n] = insert;

}

int string_input_menu_item(WINDOW *menu, char *option, int option_num, int max_length, char *return_val, bool maskInput) {
    int move = regular_clickable(menu, option, option_num);
    if(move != KEY_RIGHT) {
        return move;
    }

    curs_set(1);
    mvwchgat(menu, option_num + option_y_offset, sub_option_x, max_length, A_UNDERLINE, 0, NULL);
    wprintw(menu, "%s", return_val);
    

    int input;
    int cur_pos = (int)strlen(return_val) - 1;
    wmove(menu, option_num + option_y_offset, sub_option_x + cur_pos + 1);
    while(true) {
        wchgat(menu, max_length - (int)strlen(return_val), A_UNDERLINE, 0, NULL);
        input = wgetch(menu);
        if(input == 10) {
            curs_set(0);
            return KEY_LEFT;
        } else if(input == KEY_BACKSPACE && cur_pos >= 0) {
            remove_at(return_val, cur_pos);
            mvwprintw(menu, option_num + option_y_offset, sub_option_x + cur_pos,"%s", (return_val + cur_pos));
            mvwprintw(menu, option_num + option_y_offset, sub_option_x + (int)strlen(return_val), " ");
            wmove(menu, option_num + option_y_offset, sub_option_x + cur_pos);
            --cur_pos;
        } else if(input == KEY_DC && cur_pos < ((int)strlen(return_val) - 1)) {
            remove_at(return_val, cur_pos + 1);
            mvwprintw(menu, option_num + option_y_offset, sub_option_x + cur_pos + 1, "%s", (return_val + cur_pos + 1));
            mvwprintw(menu, option_num + option_y_offset, sub_option_x + (int)strlen(return_val), " ");
            wmove(menu, option_num + option_y_offset, sub_option_x + cur_pos);

        } else if(input == KEY_RIGHT && cur_pos <((int)strlen(return_val) - 1)) {
            ++cur_pos;
            wmove(menu, option_num + option_y_offset, sub_option_x + cur_pos);
        } else if (input == KEY_LEFT && cur_pos >= 0) {
            --cur_pos;
            wmove(menu, option_num + option_y_offset, sub_option_x + cur_pos + 1);
        } else if (isprint(input) && (int)strlen(return_val) < max_length) {
            ++cur_pos;
            add_at(return_val, input, cur_pos);
            if(!maskInput) {
                mvwprintw(menu, option_num + option_y_offset, sub_option_x, "%s", return_val);   
            } else {
                mvwaddch(menu, option_num + option_y_offset, sub_option_x + cur_pos, ACS_BULLET);
            }
            wmove(menu, option_num + option_y_offset, sub_option_x + cur_pos);
        }
    }
}
#pragma endregion 