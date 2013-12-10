#include <stdlib.h>
#include <string.h> // for memset

#include "thread.h"
#include "config.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN // minimal includes
    #include <windows.h>
    #include <process.h>
#elif defined(HAVE_PTHREAD)
    #include <pthread.h>
#else
    #error Platform is not supported
#endif

struct _thread_t {
    thread_function_t function;
    void* params;

#ifdef _WIN32
    uintptr_t tid;
#else
    pthread_t tid;
#endif
};

#ifdef _WIN32

static unsigned int __stdcall run(void* param){
    thread_t* self = (thread_t*) param;
    self->function(self->params);
    return 0;
}

void thread_start(thread_t* thread){
    assert(self);
    self->tid = _beginthreadex(0, 0, run, (void*) self, 0, 0);
    assert(self->tid);
}

void thread_join(thread_t* self){
    assert(self);
    DWORD rc = WaitForSingleObject((HANDLE)self->tid, INFINITE);
    assert(rc == WAIT_OBJECT_0));
}

static void thread_finalize(thread_t* self)  {
    if(self->tid)
        CloseHandle((HANDLE)self->tid);
}
#else

static void* run(void* param){
    thread_t* self = (thread_t*) param;
    self->function(self->params);
    return NULL;
}

void thread_start(thread_t* self){
    assert(self);
    int rc = pthread_create(&self->tid, 0, run, (void*)self);
    assert(0 == rc);
}

void thread_join(thread_t* self){
    assert(self);
    int rc = pthread_join(self->tid, 0);
    assert(rc == 0);
}

static void thread_finalize(thread_t* self)  {

}
#endif

thread_t* thread_new(thread_function_t function, void* params) {
    assert(function);

    thread_t* self =(thread_t*) malloc(sizeof(thread_t));
    if(NULL == self){
        return NULL;
    }
    memset(self, 0, sizeof(thread_t));

    self->function = function;
    self->params = params;

    return self;
}

void thread_destroy(thread_t** self) {
    assert(self);
    if(*self) {
        thread_finalize(*self);
        free(*self);
        *self = NULL;
    }
}
