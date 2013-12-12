#ifndef AOC_CONDITION_H
#define AOC_CONDITION_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Condition varable class struct
typedef struct _condition_t condition_t;

// allocate new condition variable
AOC_EXPORT condition_t* condition_new(void);

// deallocate resources
AOC_EXPORT void condition_destroy(condition_t** condition);

// acquire condition variable (the same as mutex->lock())
AOC_EXPORT void condition_acquire(condition_t* condition);

// release condition variable (the same as mutex->unlock())
AOC_EXPORT void condition_release(condition_t* condition);

// release acqured condition variable and sleep till someone calls 'condition_notify'
AOC_EXPORT void condition_wait(condition_t* condition);

// notify a waiter he can awake and acquire condition variable again
AOC_EXPORT void condition_notify(condition_t* condition);

#ifdef __cplusplus
}
#endif

#endif // AOC_CONDITION_H
