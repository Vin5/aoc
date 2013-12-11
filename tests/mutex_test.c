#include <stdlib.h>

#include "boolean.h"
#include "mutex.h"
#include "thread.h"
#include "tests.h"

#define THRESHOLD 10000000
#define THREAD_COUNT 4

static mutex_t* mutex = NULL;

void setup() {
    mutex = mutex_new();
}

void teardown() {
    mutex_destroy(&mutex);
}

void thread_func(void* param) {
    int* value = (int*)param;
    while(true) {
        mutex_lock(mutex);
        if(*value >= THRESHOLD) {
            mutex_unlock(mutex);
            break;
        }
        (*value)++;
        mutex_unlock(mutex);
    }

}



void test_mutex() {
    int value = 0;

    int i;

    thread_t* threads[THREAD_COUNT];


    for(i = 0; i < THREAD_COUNT; i++) {
        threads[i] = thread_new(thread_func, (void*)&value);
        thread_start(threads[i]);
    }

    for(i = 0; i < THREAD_COUNT; i++) {
        thread_join(threads[i]);
        thread_destroy(&threads[i]);
    }

    CHECK_EQ(value, THRESHOLD);
}


int main(int argc, char* argv[]) {
    test_setup(setup);
    test_teardown(teardown);

    test_register(test_mutex);

    return test_run();
}

