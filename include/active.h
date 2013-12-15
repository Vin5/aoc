#ifndef AOC_ACTIVE_H
#define AOC_ACTIVE_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct _active_t aoc_active_t;

typedef void(*active_function_t)(void*);

typedef void (*active_callback_t)(void);

AOC_EXPORT aoc_active_t* aoc_active_new(void);

AOC_EXPORT void aoc_active_send(aoc_active_t *object, active_function_t function, void* params, active_callback_t callback);

AOC_EXPORT void aoc_active_destroy(aoc_active_t** object);

#ifdef __cplusplus
}
#endif

#endif // AOC_ACTIVE_H
