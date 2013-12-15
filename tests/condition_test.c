#include <stdlib.h>

#include "boolean.h"
#include "condition.h"
#include "thread.h"
#include "tests.h"

#define HORSES_COUNT 4

static aoc_condition_t* finish_condition = NULL;
static aoc_condition_t* barier_condition = NULL;

void setup() {
    finish_condition = aoc_condition_new();
    barier_condition = aoc_condition_new();
}

void teardown() {
    aoc_condition_destroy(&finish_condition);
    aoc_condition_destroy(&barier_condition);
}

void thread_horse(void* param) {
    int* value = (int*)param;
    aoc_condition_acquire(barier_condition);
    (*value)++;
    aoc_condition_release(barier_condition);
    aoc_condition_notify(barier_condition); // jump over barier

    aoc_condition_acquire(finish_condition);
    aoc_condition_wait(finish_condition);   // wait for arbiter's notification
    aoc_condition_release(finish_condition);
}

void thread_arbiter(void* param) {
    int* value = (int*)param;
    int i;
    aoc_condition_acquire(barier_condition);
    while(*value != HORSES_COUNT)
        aoc_condition_wait(barier_condition);
    aoc_condition_release(barier_condition);

    for (i = 0; i < HORSES_COUNT; i++)
        aoc_condition_notify(finish_condition);
}

void test_condition() {
    int value = 0;

    int i;

    aoc_thread_t* horses[HORSES_COUNT];

    aoc_thread_t* arbiter = aoc_thread_new(thread_arbiter, (void*)&value);

    aoc_thread_start(arbiter);

    for(i = 0; i < HORSES_COUNT; i++) {
        horses[i] = aoc_thread_new(thread_horse, (void*)&value);
        aoc_thread_start(horses[i]);
    }

    for(i = 0; i < HORSES_COUNT; i++) {
        aoc_thread_join(horses[i]);
        aoc_thread_destroy(&horses[i]);
    }

    aoc_thread_join(arbiter);
    aoc_thread_destroy(&arbiter);

    CHECK_EQ(1, 1); // if we are here - all threads are finished correctly
}


int main(int argc, char* argv[]) {
    test_setup(setup);
    test_teardown(teardown);

    test_register(test_condition);

    return test_run();
}


