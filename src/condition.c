#include <stdlib.h>
#include <assert.h>

#include "condition.h"
#include "mutex.h"
#include "config.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/*
    Windows platform doesn't have such threading primitive
    as conditional variable, so let's try to implement it ourselves
*/
struct _condition_t {
    aoc_mutex_t* mutex;
    HANDLE condition;
};


aoc_condition_t* aoc_condition_new(void) {
    aoc_condition_t* self = (aoc_condition_t*) malloc(sizeof(aoc_condition_t));
    if(!self) {
        return NULL;
    }

    self->mutex = aoc_mutex_new();
    if(!self->mutex) {
        free(self);
        return NULL;
    }
    self->condition = CreateEvent(0, FALSE, FALSE, 0);
    return self;
}


void aoc_condition_destroy(aoc_condition_t** condition_ptr) {
    aoc_condition_t* self;

    assert(condition_ptr);

    self = *condition_ptr;
    if(self) {
        aoc_mutex_destroy(&self->mutex);
        if(self->condition)
            CloseHandle(self->condition);
        free(self);
        *condition_ptr = NULL;
    }
}


void condition_wait(aoc_condition_t* self) {
    assert(self);

    aoc_condition_release(self);
    WaitForSingleObject(self->condition, INFINITE);
    aoc_condition_acquire(self);
}


void condition_notify(aoc_condition_t* self) {
    assert(self);

    SetEvent(self->condition);
}

#elif defined(HAVE_PTHREAD)

#include <pthread.h>

struct _condition_t {
    aoc_mutex_t* mutex;
    pthread_cond_t condition;
};

aoc_condition_t* aoc_condition_new(void) {
    aoc_condition_t* self = (aoc_condition_t*) malloc(sizeof(aoc_condition_t));
    if(!self) {
        return NULL;
    }

    self->mutex = aoc_mutex_new();
    if(!self->mutex) {
        free(self);
        return NULL;
    }

    pthread_cond_init(&self->condition, NULL);
    return self;
}


void aoc_condition_destroy(aoc_condition_t** condition_ptr) {
    int ret;
    aoc_condition_t* self;

    assert(condition_ptr);

    ret = 0;
    self = *condition_ptr;

    if(self) {
        do {
            ret = pthread_cond_destroy(&self->condition);
        } while(ret != 0);  // wait while another thread is waiting on condition variable

        aoc_mutex_destroy(&self->mutex);

        free(self);
        *condition_ptr = NULL;
    }
}


void aoc_condition_wait(aoc_condition_t* self) {
    int rc;
    assert(self);
    rc = pthread_cond_wait(&self->condition, aoc_mutex_native(self->mutex));
    assert(0 == rc);
}


void aoc_condition_notify(aoc_condition_t* self) {
    int rc;
    assert(self);
    rc = pthread_cond_signal(&self->condition);
    assert(0 == rc);
}

#else
#error Platform is not supported
#endif


void aoc_condition_acquire(aoc_condition_t* self) {
    assert(self);
    aoc_mutex_lock(self->mutex);
}


void aoc_condition_release(aoc_condition_t* self) {
    assert(self);
    aoc_mutex_unlock(self->mutex);
}
