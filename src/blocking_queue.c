#include <stdlib.h>
#include <assert.h>

#include "blocking_queue.h"
#include "condition.h"
#include "queue.h"

struct _blocking_queue_t {
    queue_t* queue;
    condition_t* condition;
};

blocking_queue_t* blocking_queue_new(void) {
    blocking_queue_t* self = (blocking_queue_t*) malloc(sizeof(blocking_queue_t));
    if(!self) {
        return NULL;
    }

    self->queue = queue_new();
    if(!self->queue) {
        free(self);
        return NULL;
    }

    self->condition = condition_new();
    if(!self->condition) {
        queue_destroy(&self->queue);
        free(self);
        return NULL;
    }
    return self;
}

void blocking_queue_push(blocking_queue_t* self, void* value) {
    assert(self);
    assert(value);

    condition_acquire(self->condition);
    queue_push(self->queue, value);
    condition_notify(self->condition);
    condition_release(self->condition);
}

void* blocking_queue_pull(blocking_queue_t* self) {
    assert(self);

    void* element = NULL;

    condition_acquire(self->condition);

    while(queue_is_empty(self->queue))
        condition_wait(self->condition);
    element = queue_pull(self->queue);

    condition_release(self->condition);
    return element;
}

void* blocking_queue_try_pull(blocking_queue_t* self) {
    assert(self);

    void* element = NULL;

    condition_acquire(self->condition);

    if(!queue_is_empty(self->queue))
        element = queue_pull(self->queue);

    condition_release(self->condition);
    return element;
}

void blocking_queue_destroy(blocking_queue_t** queue) {
    assert(queue);

    blocking_queue_t* self = *queue;
    if(self) {
        queue_destroy(&self->queue);
        condition_destroy(&self->condition);
        free(self);
        *queue = NULL;
    }
}

