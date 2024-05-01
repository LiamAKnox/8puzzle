#include <stdbool.h>


struct Astar_node;//the required information needed to apply the A* search algorithm on a 3x3 board
struct Astar_list;//a list of Astar_nodes

//Astar_list_init() creates an empty Astar_list pointer
//effects: allocates heap memory [client must call destroy_list]
struct Astar_list *Astar_list_init();

//Astar_node_init(layout, moves, distance) creates an Astar_node pointer with the given parameters
//layout is the 3x3 board represented as an integer, moves is how many moves have been applied to the board
//and distance is the heuristic used in the A* search algorithm
//effects: allocates heap memory [client must call free]
//requires: layout must represent a 3x3 board, moves and distance must both be positive
struct Astar_node *Astar_node_init(int layout, int moves, int distance);

//list_add(list, node) attempts to add an Astar_node into an Astar_list
//if the node's layout is unique to the other nodes in the list it will be added
//if the node's layout isn't unique, but it has a smaller moves count, the node with the higher 
//move count will be removed and *node will be added
//otherwise it will node be added
bool list_add(struct Astar_list *list, struct Astar_node *node);

//list_remove_smallest(list) will find the node with the smallest (moves + heuristic)
//it will return this node and remove it from the list
//requires: *list must not be empty
struct Astar_node *list_remove_smallest(struct Astar_list *list);

//list_remove_idx(list, idx) removes the node at index idx from *list
//requires: idx must be a valid index in *list
void list_remove_idx(struct Astar_list *list, int idx);

//in_list(list, node) returns a boolean if node is already in list
//specifically, if the layout of *node is unique compared to the nodes in *list it returns false
//if the layout isn't unique, but *node has a smaller move count that the other node, the other node is removed
//and the function returns false
//otherwise, it returns true
bool in_list(struct Astar_list *list, struct Astar_node *node);

//destroy_list(list) frees all memory used in *list
void destroy_list(struct Astar_list *list);

//get_layout(node) returns the integer representation of a board
int get_layout(struct Astar_node *node);

//get_moves(node) returns the moves applied to a node's board
int get_moves(struct Astar_node *node);

//get_distance(node) returns the heuristic value stored in *node
int get_distance(struct Astar_node *node);