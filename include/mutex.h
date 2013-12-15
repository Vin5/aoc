#ifndef AOC_MUTEX_H
#define AOC_MUTEX_H

#include "boolean.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Mutex class structure
typedef struct _mutex_t aoc_mutex_t;

// allocate resources for mutex object
AOC_EXPORT aoc_mutex_t* aoc_mutex_new(void);

// lock
AOC_EXPORT void aoc_mutex_lock(aoc_mutex_t* mutex);

// unlock
AOC_EXPORT void aoc_mutex_unlock(aoc_mutex_t* mutex);

// deallocate resources
AOC_EXPORT void aoc_mutex_destroy(aoc_mutex_t** mutex);

// get a pointer to platform specific underlying mutex primitive
AOC_EXPORT void* aoc_mutex_native(aoc_mutex_t* mutex);

#ifdef __cplusplus
}
#endif

#endif // AOC_MUTEX_H
