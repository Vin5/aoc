#include <stdlib.h>

#include "queue.h"
#include "tests.h"

static aoc_queue_t* queue;

void setup() {
    queue = aoc_queue_new();
    FATAL_TRUE(queue);
}

void teardown() {
    aoc_queue_destroy(&queue);
}

void test_common_cases() {
    int i;
    int* test_val;

    int values [256];


    for(i = 0; i < 256; i++) {
        values[i] = i;
        aoc_queue_push(queue, &values[i]);
    }

    CHECK_FALSE(aoc_queue_is_empty(queue));

    CHECK_EQ(aoc_queue_size(queue), 256);

    for(i = 0; i < 256; i++) {
        test_val  = aoc_queue_pull(queue);

        FATAL_TRUE(test_val);

        CHECK_EQ(*test_val, i);

    }

    CHECK_TRUE(aoc_queue_is_empty(queue));
}

void test_edge_cases() {
    int* value;
    value = (int*)malloc(sizeof(int));
    *value = 5;

    CHECK_TRUE(aoc_queue_is_empty(queue));

    CHECK_FALSE(aoc_queue_pull(queue));

    CHECK_EQ(aoc_queue_size(queue), 0);

    aoc_queue_push(queue, value);

    CHECK_EQ(aoc_queue_size(queue), 1);

    CHECK_EQ(aoc_queue_pull(queue), value);

    free(value);

    CHECK_EQ(aoc_queue_size(queue), 0);

    CHECK_FALSE(aoc_queue_pull(queue));

    CHECK_TRUE(aoc_queue_is_empty(queue));
}


int main(int argc, char* argv[]) {
    test_setup(setup);
    test_teardown(teardown);

    test_register(test_common_cases);
    test_register(test_edge_cases);

    return test_run();
}
