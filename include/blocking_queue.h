#ifndef AOC_BLOCKING_QUEUE_H
#define AOC_BLOCKING_QUEUE_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// Blocking queue class struct
typedef struct _blocking_queue_t aoc_blocking_queue_t;


// allocates memory for a new blocking queue object
AOC_EXPORT aoc_blocking_queue_t* aoc_blocking_queue_new(void);


// enqueues a value element to the queue
AOC_EXPORT void aoc_blocking_queue_push(aoc_blocking_queue_t* queue, void* value);


// dequeues a value from the queue and returns it
// blocks the calling thread if the blocking queue is empty
// until some thread puts any value to the queue
AOC_EXPORT void* aoc_blocking_queue_pull(aoc_blocking_queue_t* queue);

// dequeues a value from the queue and returns it
// NOTE: doesn't block the calling thread and returns NULL
// if there are not elements available
AOC_EXPORT void* aoc_blocking_queue_try_pull(aoc_blocking_queue_t* queue);


// frees allocated resources
AOC_EXPORT void aoc_blocking_queue_destroy(aoc_blocking_queue_t** queue);


#ifdef __cplusplus
}
#endif

#endif // AOC_BLOCKING_QUEUE_H
