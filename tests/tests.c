#include "tests.h"

#include <stdio.h>
#include <stdlib.h>

#define TESTS_COUNT 256

#define SETUP() \
    do {if(test_environment()->setup) test_environment()->setup(); } while(0)

#define TEAR_DOWN() \
    do {if(test_environment()->teardown) test_environment()->teardown(); } while(0)

typedef struct _testing_environment_t {
    test_function_t tests[TESTS_COUNT];
    test_function_t setup;
    test_function_t teardown;
    int registered;
    int failed_assertations;
} testing_environment_t;

static testing_environment_t* test_environment(void) {
    static testing_environment_t* self = NULL;
    if(self) {
        return self;
    }
    self = calloc(1, sizeof(testing_environment_t));
    if(self == NULL) {
        fprintf(stderr, "Cannot initialize testing environment: ENOMEM");
        exit(1);
    }
    return self;
}

void test_assert(int result, int line, char* expression, assertation_level_e level) {
    if(!result) {
        testing_environment_t* self = test_environment();
        self->failed_assertations++;
        switch(level) {
        case CHECK:
            printf("Checking failed at line %d:\n\t%s\n", line, expression);
            break;
        case ASSERT:
            printf("Assertation failed at line %d:\n\t%s\n", line, expression);
            break;
        case FATAL:
            printf("Fatal assertation failed at line %d:\n\t%s\n", line, expression);
            TEAR_DOWN();
            exit(self->failed_assertations);
            break;
        }

    }
}

void test_setup(test_function_t setup) {
    testing_environment_t* self = test_environment();
    self->setup = setup;
}

void test_teardown(test_function_t teardown) {
    testing_environment_t* self = test_environment();
    self->teardown = teardown;
}

void test_register(test_function_t foo) {
    testing_environment_t* self = test_environment();
    if(self->registered < TESTS_COUNT) {
        self->tests[self->registered] = foo;
        self->registered++;
    }
}

int test_run(void) {
    int i;
    testing_environment_t* self = test_environment();

    for(i = 0; i < self->registered; i++) {
        SETUP();
            self->tests[i]();
        TEAR_DOWN();
    }

    return self->failed_assertations;
}

