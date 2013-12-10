#include <stdlib.h>

#include "active.h"
#include "thread.h"
#include "blocking_queue.h"

struct _active_t {
    thread_t* thread;
    blocking_queue_t* queue;
};

static void done(void) {

}

static void dispatch(void* param) {
    assert(param);

    blocking_queue_t* queue = (blocking_queue_t*) param;
    active_function_t function = NULL;
    while((function = blocking_queue_pull(queue)) != done)
        function();
}

active_t* active_new(void) {
    active_t* self = (active_t*) malloc(sizeof(active_t));
    if(!self) {
        return NULL;
    }

    self->queue = blocking_queue_new();
    if(!self->queue) {
        free(self);
        return NULL;
    }

    self->thread = thread_new(dispatch, self->queue);
    if(!self->thread) {
        blocking_queue_destroy(&self->queue);
        free(self);
        return NULL;
    }

    thread_start(self->thread);

    return self;
}

void active_send(active_t* self, active_function_t function) {
    assert(self);

    blocking_queue_push(self->queue, function);
}

void active_destroy(active_t** object_ptr) {
    assert(object_ptr);
    active_t* self = *object_ptr;
    if(self) {
        active_send(self, done);
        thread_join(self->thread);

        thread_destroy(&self->thread);
        blocking_queue_destroy(&self->queue);
        free(self);
        *object_ptr = NULL;
    }
}
