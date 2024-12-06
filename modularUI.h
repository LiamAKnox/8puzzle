#ifndef BF84332E_23FD_48F4_8CE0_367A563A54A5
#define BF84332E_23FD_48F4_8CE0_367A563A54A5

#include <ncurses.h>
#include <stdbool.h>


struct MENU;


void build_UI(WINDOW *menu, char *title, char **options, int options_count, int max_sub_count, int longest_option, int longest_sub_option);

struct MENU *new_menu(char *title);
int run_menu(struct MENU *menu);
void add_push_button(struct MENU *menu, char *name);
void add_menu_button(struct MENU *menu, char *name, char **sub_options, int *return_destination);
void add_input_spinner(struct MENU *menu, char *name, int max_val, int min_val, int *return_destination);
void add_text_input(struct MENU *menu, char *name, int shortest, int longest, bool password, char *return_destination);

int regular_clickable(WINDOW *menu, char *option, int option_num);//this will just be highlighting the option, 

int options_menu_item(WINDOW *menu, char *option, int option_num, char **sub_options, int sub_options_count, int *chosen_sub);//returned int is chosen sub_option

int number_menu_item(WINDOW *menu, char *option, int option_num, int standard, int max, int min, int *current_num);

int string_input_menu_item(WINDOW *menu, char *option, int option_num, int max_length, char *return_val, bool maskInput);

void clear_right_side(struct MENU *menu);

void destroy_menu(struct MENU *menu);

//void destroy_menu_item(struct MENU_ITEM *menu_item);

#endif /* BF84332E_23FD_48F4_8CE0_367A563A54A5 */
