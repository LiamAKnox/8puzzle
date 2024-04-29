#include <stdlib.h>
#include <assert.h>

#include "queueADT.h"

struct queue {
    int *order;
    int len;
    int max_len;
};

struct queue *queue_init() {
    struct queue *q = malloc(sizeof(struct queue));
    assert(q);
    q->len = 0;
    q->max_len = 1;
    q->order = malloc(q->max_len * sizeof(int));
    assert(q->order);
    return q;
}

void enqueue(struct queue *q, int data) {
    assert(q);
    if (q->len == q->max_len) {
        q->max_len *=2;
        q->order = realloc(q->order, q->max_len * sizeof(int));
        assert(q->order);
    }
    *(q->order + q->len) = data;
    q->len++;
}


void queue_destroy(struct queue *q) {
    assert(q);
    free(q->order);
    free(q);
}

bool is_empty(struct queue *q) {
    assert(q);
    return !(q->len);

}


int dequeue(struct queue *q) {
    assert(q);
    assert(!(is_empty(q)));
    int data = *(q->order);
    for (int i = 0; i < q->len - 1; i++) {
        *(q->order + i) = *(q->order + (i + 1));
    }
    q->len--;
    return data;
}


bool in_queue(struct queue *q, int data) {
    assert(q);
    assert(data);

    for (int i = 0; i < q->len; i++) {
        if (*(q->order + i) - data == 0) {
            return true;
        }
    }
    return false;
}



int queue_size(struct queue *q) {
    assert(q);
    return q->len;
}



void queue_cat(struct queue *dest, struct queue *add) {
    assert(dest);
    assert(add);
    for (int i = 0; i < add->len; i++) {
        enqueue(dest, *(add->order + i));
    }
}

int queue_front(struct queue *q) {
    return *(q->order);
}


struct queue *queue_clone(struct queue *q) {
    struct queue *new_q = malloc(sizeof(struct queue));
    new_q->len = q->len;
    new_q->max_len = q->max_len;
    new_q->order = malloc(new_q->max_len * sizeof(int));
    for (int i = 0; i < new_q->len; i++) {
        *(new_q->order + i) = *(q->order + i);
    }
    return new_q;
}

