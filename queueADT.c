//includes go here:
#include "queueADT.h"
#include <stdlib.h>
#include <assert.h>

struct queue {
    void **order;
    int len;
    int max_len;
    int size;
};


struct queue *queue_init(int size) {
    struct queue *q = malloc(sizeof(struct queue));
    assert(q);
    q->len = 0;
    q->max_len = 1;
    q->size = size;
    q->order = malloc(q->max_len * q->size);
    assert(q->order);
    return q;
}

void enqueue(struct queue *q, void *data) {
    assert(q);
    assert(data);
    if (q->len == q->max_len) {
        q->max_len *=2;
        q->order = realloc(q->order, q->max_len * q->size);
        assert(q->order);
    }
    *(q->order + q->len) = data;
    q->len++;
}


void queue_destroy(struct queue *q, void(*destroy_data)(void *)) {
    assert(q);
    assert(destroy_data);
    for (int i = 0; i < q->len; i++) {
        destroy_data(*(q->order + i));
    }
    free(q->order);
    free(q);
}

bool is_empty(struct queue *q) {
    assert(q);
    return !(q->len);

}


void *dequeue(struct queue *q) {
    assert(q);
    assert(!(is_empty(q)));
    struct board *board = *(q->order);
    for (int i = 0; i < q->len - 1; i++) {
        *(q->order + i) = *(q->order + (i + 1));
    }
    q->len--;
    return board;
}


bool in_queue(struct queue *q, void *data, int(*comp)(void *, void *)) {
    assert(q);
    assert(data);
    assert(comp);

    for (int i = 0; i < q->len; i++) {
        if (comp (*(q->order + i), data) == 0) {
            return true;
        }
    }
    return false;
}

