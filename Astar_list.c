#include "Astar_list.h"
#include <assert.h>
#include <stdlib.h>

struct Astar_node {
    int layout;
    int moves;
    int distance;
};

struct Astar_list {
    struct Astar_node **data;
    int len;
    int max_len;
};


//add, remove, search, init

struct Astar_list *Astar_list_init() {

    struct Astar_list *list = malloc(sizeof(struct Astar_list));
    list->len = 0;
    list->max_len = 1;
    list->data = malloc(sizeof(struct Astar_node *) * list->max_len);
    return list;
}

struct Astar_node *Astar_node_init(int layout, int moves, int distance) {
    struct Astar_node *node = malloc(sizeof(struct Astar_node));
    node->distance = distance;
    node->layout = layout;
    node->moves = moves;
    return node;
}



bool list_add(struct Astar_list *list, struct Astar_node *node) {
    assert(list);
    assert(node);
    if (in_list(list, node)) {
        return false;
    }

    if (list->len == list->max_len) {
        list->max_len *=2;
        list->data = realloc(list->data, list->max_len * sizeof(struct Astar_node *));
    }
    *(list->data + list->len) = node;
    list->len++;
    return true;
}

struct Astar_node *list_remove_smallest(struct Astar_list *list) {
    assert(list);
    assert(list->len > 0);
    struct Astar_node *smallest = *(list->data);
    int idx = 0;
    int test = smallest->moves + smallest->distance;
    for (int i = 1; i < list->len; i++) {
        if ((*(list->data + i))->moves +  (*(list->data + i))->distance < smallest->moves + smallest->distance) {
            smallest = *(list->data + i);
            test = (*(list->data + i))->moves +  (*(list->data + i))->distance;
            idx = i;
        }
    }
    
    list_remove_idx(list, idx);
    
    return smallest;
}

void list_remove_idx(struct Astar_list *list, int idx) {
    assert(list);
    for (int i = idx; i < list->max_len - 1; i++) {
        *(list->data + i) = *(list->data + i + 1);
    }
    list->len--;
}


//in_list checks if its in list
//even if it is in the list, if its corresponding node in the list has more moves it removes that node and returns false
bool in_list(struct Astar_list *list, struct Astar_node *node) {
    assert(list);
    for (int i = 0; i < list->len; i++) {
        if ((*(list->data + i))->layout == node->layout) {
            if (node->moves < (*(list->data + i))->moves) {
                free(*(list->data + i));
                list_remove_idx(list, i);
                return false;
            } else {
                return true;
            }
        }
    }
    return false;

}

void destroy_list(struct Astar_list *list) {
    assert(list);
    for (int i = 0; i < list->len; i++) {
        free(*(list->data + i)); 
    }
    free(list->data);
    free(list);
}

int get_layout(struct Astar_node *node) {
    assert(node);
    return node->layout;
}

int get_moves(struct Astar_node *node) {
    assert(node);
    return node->moves;
}

int get_distance(struct Astar_node *node) {
    assert(node);
    return node->distance;
}