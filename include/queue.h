#ifndef AOC_QUEUE_H
#define AOC_QUEUE_H

#include "boolean.h"
#include "config.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// FIFO class structure
typedef struct _queue_t queue_t;

// create new queue object
AOC_EXPORT queue_t* queue_new(void);

// destroy queue object, doesn't free elements it still holding
AOC_EXPORT void queue_destroy(queue_t** queue_ptr);

// check if the queue is empty
AOC_EXPORT bool queue_is_empty(queue_t* queue);

// enqueue an element
AOC_EXPORT void queue_push(queue_t* queue, void* element);

// dequeue an element, returns NULL in case queue is empty
AOC_EXPORT void* queue_pull(queue_t* queue);

// get size of the queue
AOC_EXPORT size_t queue_size(queue_t* queue);


#ifdef __cplusplus
}
#endif

#endif // AOC_QUEUE_H
