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
    mutex_t* mutex;
    HANDLE condition;
};


condition_t* condition_new(void) {
    condition_t* self = (condition_t*) malloc(sizeof(condition_t));
    if(!self) {
        return NULL;
    }

    self->mutex = mutex_new();
    if(!self->mutex) {
        free(self);
        return NULL;
    }
    self->condition = CreateEvent(0, FALSE, FALSE, 0);
    return self;
}


void condition_destroy(condition_t** condition_ptr) {
    assert(condition_ptr);
    condition_t* self = *condition_ptr;
    if(self) {
        mutex_destroy(&self->mutex);
        if(self->condition)
            CloseHandle(self->condition);
        free(self);
        *condition_ptr = NULL;
    }
}


void condition_wait(condition_t* self) {
    assert(self);
    assert(mutex_is_locked(self->mutex));

    condition_release(self);
    WaitForSingleObject(self->condition, INFINITE);
    condition_acquire(self);
}


void condition_notify(condition_t* self) {
    assert(self);
    assert(mutex_is_locked(self->mutex));

    SetEvent(self->condition);
}

#elif defined(HAVE_PTHREAD)

#include <pthread.h>

struct _condition_t {
    mutex_t* mutex;
    pthread_cond_t condition;
};

condition_t* condition_new(void) {
    condition_t* self = (condition_t*) malloc(sizeof(condition_t));
    if(!self) {
        return NULL;
    }

    self->mutex = mutex_new();
    if(!self->mutex) {
        free(self);
        return NULL;
    }

    pthread_cond_init(&self->condition, NULL);
    return self;
}


void condition_destroy(condition_t** condition_ptr) {
    assert(condition_ptr);

    int ret = 0;
    condition_t* self = *condition_ptr;

    if(self) {
        do {
            ret = pthread_cond_destroy(&self->condition);
        } while(ret != 0);  // wait while another thread is waiting on condition variable

        mutex_destroy(&self->mutex);

        free(self);
        *condition_ptr = NULL;
    }
}


void condition_wait(condition_t* self) {
    assert(self);
    int rc = pthread_cond_wait(&self->condition, mutex_native(self->mutex));
    assert(0 == rc);
}


void condition_notify(condition_t* self) {
    assert(self);
    int rc = pthread_cond_signal(&self->condition);
    assert(0 == rc);
}

#else
#error Platform is not supported
#endif


void condition_acquire(condition_t* self) {
    assert(self);
    mutex_lock(self->mutex);
}


void condition_release(condition_t* self) {
    assert(self);
    mutex_unlock(self->mutex);
}
