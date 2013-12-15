#ifndef AOC_THREAD_H
#define AOC_THREAD_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Simple threading functionality

// thread class structure
typedef struct _thread_t aoc_thread_t;

// thread executable function type
typedef void(*thread_function_t)(void*);

// create new thread object in non executing state
AOC_EXPORT aoc_thread_t* aoc_thread_new(thread_function_t function, void* params);

// begin executing its function in different OS thread
AOC_EXPORT void aoc_thread_start(aoc_thread_t* thrd);

// wait for thread execution is finished
AOC_EXPORT void aoc_thread_join(aoc_thread_t* thrd);

// free resources allocated for 'thrd'
AOC_EXPORT void aoc_thread_destroy(aoc_thread_t** thrd);


#ifdef __cplusplus
}
#endif

#endif // AOC_THREAD_H
