#include <stdbool.h>
#include "8puzzle.h"

//A Queue ADT for integers
struct queue;

//queue_init() creates a new empty queue adt
//effects: allocates heap memory [client must call queue_destroy]
struct queue *queue_init();


//enqueue(q, data) adds a new element onto the end of the queue
//effects: mutates *q
void enqueue(struct queue *q, int data);

//queue_destroy(q) releases all resources used by *q
//effects: invalidates *q
void queue_destroy(struct queue *q);

//dequeue(q) returns the first piece of data from *q
//effects: mutates *q
//requires: queue must not be empty
int dequeue(struct queue *q);


//is_empty(q) returns true if *q is empty, and false otherwise
bool is_empty(struct queue *q);

//in_queue(q, data) checks if *data is already in *q
bool in_queue(struct queue *q, int data);


//queue_size(q) returns the number of elements in *q
int queue_size(struct queue *q);

//queue_cat(dest, add) enqueues all values of *add into *q
//order of *add elements are maintained and *add is not invalidate
//effects: mutates *dest 
void queue_cat(struct queue *dest, struct queue *add);

//queue_clone(q) creates and returns a duplicate of *q
struct queue *queue_clone(struct queue *q);

