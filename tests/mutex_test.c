#include <stdlib.h>

#include "boolean.h"
#include "mutex.h"
#include "thread.h"
#include "tests.h"

#define THRESHOLD 10000000
#define THREAD_COUNT 4

static aoc_mutex_t* mutex = NULL;

void setup() {
    mutex = aoc_mutex_new();
}

void teardown() {
    aoc_mutex_destroy(&mutex);
}

void thread_func(void* param) {
    int* value = (int*)param;
    while(true) {
        aoc_mutex_lock(mutex);
        if(*value >= THRESHOLD) {
            aoc_mutex_unlock(mutex);
            break;
        }
        (*value)++;
        aoc_mutex_unlock(mutex);
    }

}



void test_mutex() {
    int value = 0;

    int i;

    aoc_thread_t* threads[THREAD_COUNT];


    for(i = 0; i < THREAD_COUNT; i++) {
        threads[i] = aoc_thread_new(thread_func, (void*)&value);
        aoc_thread_start(threads[i]);
    }

    for(i = 0; i < THREAD_COUNT; i++) {
        aoc_thread_join(threads[i]);
        aoc_thread_destroy(&threads[i]);
    }

    CHECK_EQ(value, THRESHOLD);
}


int main(int argc, char* argv[]) {
    test_setup(setup);
    test_teardown(teardown);

    test_register(test_mutex);

    return test_run();
}

