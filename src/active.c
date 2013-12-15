#include <stdlib.h>
#include <assert.h>

#include "active.h"
#include "thread.h"
#include "blocking_queue.h"

struct _active_t {
    aoc_thread_t* thread;
    aoc_blocking_queue_t* queue;
};

typedef struct _active_context_t {
   active_function_t function;
   void* params;
   active_callback_t callback;
} active_context_t;


static active_context_t* active_context_new(active_function_t function, void* params, active_callback_t callback) {
    active_context_t* self = (active_context_t*) malloc(sizeof(active_context_t));
    if(!self)
        return NULL;

    self->function = function;
    self->params = params;
    self->callback = callback;

    return self;
}

static void active_context_destroy(active_context_t** self_ptr) {
    assert(self_ptr);

    if(*self_ptr) {
        free(*self_ptr);
        *self_ptr = NULL;
    }
}

static active_context_t done;

static void dispatch(void* param) {
    aoc_blocking_queue_t* queue;
    active_context_t* context;

    assert(param);

    queue = (aoc_blocking_queue_t*) param;
    context = NULL;
    while((context = aoc_blocking_queue_pull(queue)) != &done) {
        context->function(context->params);
        if(context->callback) {
            context->callback();
        }
        active_context_destroy(&context);
    }
}

aoc_active_t* aoc_active_new(void) {
    aoc_active_t* self = (aoc_active_t*) malloc(sizeof(aoc_active_t));
    if(!self) {
        return NULL;
    }

    self->queue = aoc_blocking_queue_new();
    if(!self->queue) {
        free(self);
        return NULL;
    }

    self->thread = aoc_thread_new(dispatch, self->queue);
    if(!self->thread) {
        aoc_blocking_queue_destroy(&self->queue);
        free(self);
        return NULL;
    }

    aoc_thread_start(self->thread);

    return self;
}

void aoc_active_send(aoc_active_t *self, active_function_t function, void* params, active_callback_t callback) {
    active_context_t* context;

    assert(self);
    assert(function);

    context = active_context_new(function, params, callback);
    aoc_blocking_queue_push(self->queue, context);
}

void aoc_active_destroy(aoc_active_t** object_ptr) {
    aoc_active_t* self;
    assert(object_ptr);

    self = *object_ptr;
    if(self) {
        aoc_blocking_queue_push(self->queue, &done);
        aoc_thread_join(self->thread);

        aoc_thread_destroy(&self->thread);
        aoc_blocking_queue_destroy(&self->queue);
        free(self);
        *object_ptr = NULL;
    }
}
