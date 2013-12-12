#include <stdlib.h>
#include <assert.h>

#include "active.h"
#include "thread.h"
#include "blocking_queue.h"

struct _active_t {
    thread_t* thread;
    blocking_queue_t* queue;
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
    blocking_queue_t* queue;
    active_context_t* context;

    assert(param);

    queue = (blocking_queue_t*) param;
    context = NULL;
    while((context = blocking_queue_pull(queue)) != &done) {
        context->function(context->params);
        if(context->callback) {
            context->callback();
        }
        active_context_destroy(&context);
    }
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

void active_send(active_t *self, active_function_t function, void* params, active_callback_t callback) {
    active_context_t* context;

    assert(self);
    assert(function);

    context = active_context_new(function, params, callback);
    blocking_queue_push(self->queue, context);
}

void active_destroy(active_t** object_ptr) {
    active_t* self;
    assert(object_ptr);

    self = *object_ptr;
    if(self) {
        blocking_queue_push(self->queue, &done);
        thread_join(self->thread);

        thread_destroy(&self->thread);
        blocking_queue_destroy(&self->queue);
        free(self);
        *object_ptr = NULL;
    }
}
