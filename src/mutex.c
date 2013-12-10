#include <stdlib.h>

#include "boolean.h"
#include "config.h"
#include "mutex.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct _mutex_t {
    CRITICAL_SECTION mutex;
    volatile bool locked;
};

mutex_t* mutex_new(void) {
    mutex_t* self = (mutex_t*) malloc(sizeof(mutex_t));
    if(!self) {
        return NULL;
    }

    InitializeCriticalSection(&self->mutex);
    self->locked = false;

    return self;
}

void mutex_lock(mutex_t* self) {
    assert(self);
    assert(!self->locked);

    EnterCriticalSection(&self->mutex);
    self->locked = true;
}

void mutex_unlock(mutex_t* self) {
    assert(self);
    assert(self->locked);

    self->locked = false;
    LeaveCriticalSection(&self->mutex);
}

void mutex_destroy(mutex_t** mutex_ptr) {
    assert(mutex_ptr);
    mutex_t* self = *mutex_ptr;
    if(self) {
        assert(!self->locked);
        DeleteCriticalSection(&self->mutex);
        free(self);
        self = NULL;
    }
}

#elif defined(HAVE_PTHREAD)

#include <pthread.h>

struct _mutex_t {
    pthread_mutex_t mutex;
    volatile bool locked;
};

mutex_t* mutex_new(void) {
    mutex_t* self = (mutex_t*) malloc(sizeof(mutex_t));
    if(!self) {
        return NULL;
    }
    pthread_mutex_init(&self->mutex, NULL);
    self->locked = false;
}

void mutex_lock(mutex_t* self) {
    assert(self);
    assert(!self->locked);

    int rc = pthread_mutex_lock(&self->mutex);
    assert(0 == rc);
    self->locked = true;
}

void mutex_unlock(mutex_t* self) {
    assert(self);
    assert(self->locked);

    self->locked = false;
    int rc = pthread_mutex_unlock(&self->mutex);
    assert(0 == rc);
}

void mutex_destroy(mutex_t** mutex_ptr) {
    assert(mutex_ptr);
    mutex_t* self = *mutex_ptr;
    if(self) {
        assert(!self->locked);
        pthread_mutex_destroy(&self->mutex);
        free(self);
        self = NULL;
    }
}

#else
#error Platform is not supported
#endif
