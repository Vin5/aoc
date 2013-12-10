#ifndef AOC_ACTIVE_H
#define AOC_ACTIVE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _active_t active_t;

typedef void(*active_function_t)(void);

active_t* active_new(void);

void active_send(active_function_t function);

void active_destroy(active_t** object);

#ifdef __cplusplus
}
#endif

#endif // AOC_ACTIVE_H
