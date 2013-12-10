#ifndef AOC_CONDITION_H
#define AOC_CONDITION_H


// Condition varable class struct
typedef struct _condition_t condition_t;

// allocate new condition variable
condition_t* condition_new(void);

// deallocate resources
void condition_destroy(condition_t** condition);

// acquire condition variable (the same as mutex->lock())
void condition_acquire(condition_t* condition);

// release condition variable (the same as mutex->unlock())
void condition_release(condition_t* condition);

// release acqured condition variable and sleep till someone calls 'condition_notify'
void condition_wait(condition_t* condition);

// notify a waiter he can awake and acquire condition variable again
void condition_notify(condition_t* condition);

#endif // AOC_CONDITION_H
