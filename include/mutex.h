#ifndef AOC_MUTEX_H
#define AOC_MUTEX_H

#include "boolean.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Mutex class structure
typedef struct _mutex_t mutex_t;

// allocate resources for mutex object
AOC_EXPORT mutex_t* mutex_new(void);

// lock
AOC_EXPORT void mutex_lock(mutex_t* mutex);

// unlock
AOC_EXPORT void mutex_unlock(mutex_t* mutex);

// deallocate resources
AOC_EXPORT void mutex_destroy(mutex_t** mutex);

// get a pointer to platform specific underlying mutex primitive
AOC_EXPORT void* mutex_native(mutex_t* mutex);

#ifdef __cplusplus
}
#endif

#endif // AOC_MUTEX_H
