#include <stdlib.h>
#include <assert.h>

#include "blocking_queue.h"
#include "condition.h"
#include "queue.h"

struct _blocking_queue_t {
    aoc_queue_t* queue;
    aoc_condition_t* condition;
};

aoc_blocking_queue_t* aoc_blocking_queue_new(void) {
    aoc_blocking_queue_t* self = (aoc_blocking_queue_t*) malloc(sizeof(aoc_blocking_queue_t));
    if(!self) {
        return NULL;
    }

    self->queue = aoc_queue_new();
    if(!self->queue) {
        free(self);
        return NULL;
    }

    self->condition = aoc_condition_new();
    if(!self->condition) {
        aoc_queue_destroy(&self->queue);
        free(self);
        return NULL;
    }
    return self;
}

void aoc_blocking_queue_push(aoc_blocking_queue_t* self, void* value) {
    assert(self);
    assert(value);

    //aoc_condition_acquire(self->condition);
    aoc_queue_push(self->queue, value);
    aoc_condition_notify(self->condition);
    //condition_release(self->condition);
}

void* aoc_blocking_queue_pull(aoc_blocking_queue_t* self) {
    void* element;

    assert(self);

    element = NULL;

    aoc_condition_acquire(self->condition);

    while(aoc_queue_is_empty(self->queue))
        aoc_condition_wait(self->condition);
    element = aoc_queue_pull(self->queue);

    aoc_condition_release(self->condition);
    return element;
}

void* aoc_blocking_queue_try_pull(aoc_blocking_queue_t* self) {
    void* element;

    assert(self);

    element = NULL;

    aoc_condition_acquire(self->condition);

    if(!aoc_queue_is_empty(self->queue))
        element = aoc_queue_pull(self->queue);

    aoc_condition_release(self->condition);
    return element;
}

void aoc_blocking_queue_destroy(aoc_blocking_queue_t** queue) {
    aoc_blocking_queue_t* self;

    assert(queue);

    self = *queue;
    if(self) {
        aoc_queue_destroy(&self->queue);
        aoc_condition_destroy(&self->condition);
        free(self);
        *queue = NULL;
    }
}

