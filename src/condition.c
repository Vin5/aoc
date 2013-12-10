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
    HANDLE m_event;
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
    self->m_event = CreateEvent(0, FALSE, FALSE, 0);
}


void condition_destroy(condition_t** condition_ptr) {
    assert(condition_ptr);
    condition_t* self = *condition_ptr;
    if(self) {
        mutex_destroy(self->mutex);
        if(self->m_event)
            CloseHandle(self->m_event);
        free(self);
        *condition_ptr = NULL;
    }
}


void condition_wait(condition_t* self) {
    assert(self);
    assert(mutex_is_locked(self->mutex));

    condition_release(self);
    WaitForSingleObject(self->m_event, INFINITE);
    condition_acquire(self);
}


void condition_notify(condition_t* self) {
    assert(self);
    assert(mutex_is_locked(self->mutex));

    SetEvent(self->m_event);
}

#elif defined(HAVE_PTHREAD)

#else
#error Platform is not supported
#endif


void condition_acquire(condition_t* condition) {

}


void condition_release(condition_t* condition) {

}
