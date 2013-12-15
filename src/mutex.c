#include <stdlib.h>
#include <assert.h>

#include "boolean.h"
#include "config.h"
#include "mutex.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct _mutex_t {
    CRITICAL_SECTION mutex;
};

aoc_mutex_t* aoc_mutex_new(void) {
    aoc_mutex_t* self = (aoc_mutex_t*) malloc(sizeof(aoc_mutex_t));
    if(!self) {
        return NULL;
    }

    InitializeCriticalSection(&self->mutex);
    return self;
}

void aoc_mutex_lock(aoc_mutex_t* self) {
    assert(self);
    EnterCriticalSection(&self->mutex);
}

void aoc_mutex_unlock(aoc_mutex_t* self) {
    assert(self);
    LeaveCriticalSection(&self->mutex);
}

void aoc_mutex_destroy(aoc_mutex_t** mutex_ptr) {
    aoc_mutex_t* self;

    assert(mutex_ptr);

    self = *mutex_ptr;
    if(self) {
        DeleteCriticalSection(&self->mutex);
        free(self);
        self = NULL;
    }
}

#elif defined(HAVE_PTHREAD)

#include <pthread.h>

struct _mutex_t {
    pthread_mutex_t mutex;
};

aoc_mutex_t* aoc_mutex_new(void) {
    aoc_mutex_t* self = (aoc_mutex_t*) malloc(sizeof(aoc_mutex_t));
    if(!self) {
        return NULL;
    }
    pthread_mutex_init(&self->mutex, NULL);
    return self;
}

void aoc_mutex_lock(aoc_mutex_t* self) {
    int rc;

    assert(self);
    rc = pthread_mutex_lock(&self->mutex);
    assert(0 == rc);
}

void aoc_mutex_unlock(aoc_mutex_t* self) {
    int rc;

    assert(self);
    rc = pthread_mutex_unlock(&self->mutex);
    assert(0 == rc);
}

void aoc_mutex_destroy(aoc_mutex_t** mutex_ptr) {
    aoc_mutex_t* self;

    assert(mutex_ptr);

    self = *mutex_ptr;
    if(self) {
        pthread_mutex_destroy(&self->mutex);
        free(self);
        self = NULL;
    }
}

#else
#error Platform is not supported
#endif

void* aoc_mutex_native(aoc_mutex_t* self) {
    assert(self);
    return &self->mutex;
}
