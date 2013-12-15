#include <stdlib.h>
#include <string.h> // for memset
#include <assert.h>

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
    aoc_thread_t* self = (aoc_thread_t*) param;
    self->function(self->params);
    return 0;
}

void aoc_thread_start(aoc_thread_t* self){
    assert(self);
    self->tid = _beginthreadex(0, 0, run, (void*) self, 0, 0);
    assert(self->tid);
}

void aoc_thread_join(aoc_thread_t* self){
    DWORD rc;
    assert(self);
    rc = WaitForSingleObject((HANDLE)self->tid, INFINITE);
    assert(rc == WAIT_OBJECT_0);
}

static void thread_finalize(aoc_thread_t* self)  {
    if(self->tid)
        CloseHandle((HANDLE)self->tid);
}
#else

static void* run(void* param){
    aoc_thread_t* self = (aoc_thread_t*) param;
    self->function(self->params);
    return NULL;
}

void aoc_thread_start(aoc_thread_t* self){
    int rc;
    assert(self);
    rc = pthread_create(&self->tid, NULL, run, (void*)self);
    assert(0 == rc);
}

void aoc_thread_join(aoc_thread_t* self){
    int rc;
    assert(self);
    rc = pthread_join(self->tid, NULL);
    assert(0 == rc);
}

static void thread_finalize(aoc_thread_t* self)  {

}
#endif

aoc_thread_t* aoc_thread_new(thread_function_t function, void* params) {
    aoc_thread_t* self;

    assert(function);

    self =(aoc_thread_t*) malloc(sizeof(aoc_thread_t));
    if(NULL == self){
        return NULL;
    }
    memset(self, 0, sizeof(aoc_thread_t));

    self->function = function;
    self->params = params;

    return self;
}

void aoc_thread_destroy(aoc_thread_t** self) {
    assert(self);
    if(*self) {
        thread_finalize(*self);
        free(*self);
        *self = NULL;
    }
}
