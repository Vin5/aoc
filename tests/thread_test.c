#include <stdlib.h>

#include "thread.h"
#include "tests.h"

void thread_func(void* param) {
    int* value = (int*) param;
    *value = 777;
}

void test_thread() {
    volatile int value = 5;
    thread_t* thread = thread_new(thread_func, (void*)&value);
    thread_start(thread);
    thread_join(thread);
    thread_destroy(&thread);

    CHECK_EQ(value, 777);
    CHECK_EQ(thread, NULL);
}


int main(int argc, char* argv[]) {
    test_register(test_thread);

    return test_run();
}
