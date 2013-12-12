#include <stdlib.h>

#include "boolean.h"
#include "condition.h"
#include "thread.h"
#include "tests.h"

#define HORSES_COUNT 4

static condition_t* finish_condition = NULL;
static condition_t* barier_condition = NULL;

void setup() {
    finish_condition = condition_new();
    barier_condition = condition_new();
}

void teardown() {
    condition_destroy(&finish_condition);
    condition_destroy(&barier_condition);
}

void thread_horse(void* param) {
    int* value = (int*)param;
    condition_acquire(barier_condition);
    (*value)++;
    condition_release(barier_condition);
    condition_notify(barier_condition); // jump over barier

    condition_acquire(finish_condition);
    condition_wait(finish_condition);   // wait for arbiter's notification
    condition_release(finish_condition);
}

void thread_arbiter(void* param) {
    int* value = (int*)param;
    int i;
    condition_acquire(barier_condition);
    while(*value != HORSES_COUNT)
        condition_wait(barier_condition);
    condition_release(barier_condition);

    for (i = 0; i < HORSES_COUNT; i++)
        condition_notify(finish_condition);
}

void test_condition() {
    int value = 0;

    int i;

    thread_t* horses[HORSES_COUNT];

    thread_t* arbiter = thread_new(thread_arbiter, (void*)&value);

    thread_start(arbiter);

    for(i = 0; i < HORSES_COUNT; i++) {
        horses[i] = thread_new(thread_horse, (void*)&value);
        thread_start(horses[i]);
    }

    for(i = 0; i < HORSES_COUNT; i++) {
        thread_join(horses[i]);
        thread_destroy(&horses[i]);
    }

    thread_join(arbiter);
    thread_destroy(&arbiter);

    CHECK_EQ(1, 1); // if we are here - all threads are finished correctly
}


int main(int argc, char* argv[]) {
    test_setup(setup);
    test_teardown(teardown);

    test_register(test_condition);

    return test_run();
}


