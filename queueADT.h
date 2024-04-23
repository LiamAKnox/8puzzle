//includes go here:
#include "8puzzle.h"
#include <stdbool.h>

struct queue;


//queue_init() creates a new empty queue adt
//effects: allocates heap memory [client must call queue_destroy]
struct queue *queue_init(int size);


//enqueue(q, data) adds a new board onto the end of the queue
//effects: mutates *q
void enqueue(struct queue *q, void *data);

//queue_destroy(q) releases all resources used by *q
//effects: invalidates *q
void queue_destroy(struct queue *q, void(*destroy_data)(void *));

//dequeue(q) returns the first piece of data from *q
//effects: mutates *q
//requires: queue must not be empty
void *dequeue(struct queue *q);


//is_empty(q) returns true if *q is empty, and false otherwise
bool is_empty(struct queue *q);

//in_queue(q, data) checks if *data is already in *q
bool in_queue(struct queue *q, void *data, int(*comp)(void *, void *));

