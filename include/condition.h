#ifndef AOC_CONDITION_H
#define AOC_CONDITION_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Condition varable class struct
typedef struct _condition_t aoc_condition_t;

// allocate new condition variable
AOC_EXPORT aoc_condition_t* aoc_condition_new(void);

// deallocate resources
AOC_EXPORT void aoc_condition_destroy(aoc_condition_t** condition);

// acquire condition variable (the same as mutex->lock())
AOC_EXPORT void aoc_condition_acquire(aoc_condition_t* condition);

// release condition variable (the same as mutex->unlock())
AOC_EXPORT void aoc_condition_release(aoc_condition_t* condition);

// release acqured condition variable and sleep till someone calls 'condition_notify'
AOC_EXPORT void aoc_condition_wait(aoc_condition_t* condition);

// notify a waiter he can awake and acquire condition variable again
AOC_EXPORT void aoc_condition_notify(aoc_condition_t* condition);

#ifdef __cplusplus
}
#endif

#endif // AOC_CONDITION_H
