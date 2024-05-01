//includes:
#include <stdbool.h>


struct Astar_node;
struct Astar_list;

//add, remove, search, init

struct Astar_list *Astar_list_init();

struct Astar_node *Astar_node_init(int layout, int moves, int distance);

bool list_add(struct Astar_list *list, struct Astar_node *node);

struct Astar_node *list_remove_smallest(struct Astar_list *list);

void list_remove_idx(struct Astar_list *list, int idx);

bool in_list(struct Astar_list *list, struct Astar_node *node);

void destroy_list(struct Astar_list *list);

int get_layout(struct Astar_node *node);

int get_moves(struct Astar_node *node);

int get_distance(struct Astar_node *node);