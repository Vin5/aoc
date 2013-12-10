#ifndef AOC_THREAD_H
#define AOC_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

// Simple threading functionality

// thread class structure
typedef struct _thread_t thread_t;

// thread executable function type
typedef void(*thread_function_t)(void*);

// create new thread object in non executing state
thread_t* thread_new(thread_function_t function, void* params);

// begin executing its function in different OS thread
void thread_start(thread_t* thrd);

// wait for thread execution is finished
void thread_join(thread_t* thrd);

// free resources allocated for 'thrd'
void thread_destroy(thread_t** thrd);


#ifdef __cplusplus
}
#endif

#endif // AOC_THREAD_H
