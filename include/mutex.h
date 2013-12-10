#ifndef AOC_MUTEX_H
#define AOC_MUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

// Mutex class structure
typedef struct _mutex_t mutex_t;

// allocate resources for mutex object
mutex_t* mutex_new(void);

// lock
void mutex_lock(mutex_t* mutex);

// unlock
void mutex_unlock(mutex_t* mutex);

// deallocate resources
void mutex_destroy(mutex_t** mutex);

#ifdef __cplusplus
}
#endif

#endif // AOC_MUTEX_H
