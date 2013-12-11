#ifndef AOC_TESTS_H
#define AOC_TESTS_H

// Simple testing framework

typedef void(*test_function_t)();

typedef enum _assertation_level_e {
    CHECK,
    ASSERT,
    FATAL
} assertation_level_e;

void test_assert(int result, int line, char* expresssion,  assertation_level_e level);


// CHECK_* macro performs expression result checking
// current test won't be interrupted even though checking's failed
#define CHECK_TRUE(expression) test_assert(!!(expression),__LINE__,#expression, CHECK)

#define CHECK_FALSE(expression) test_assert(!(expression),__LINE__,#expression, CHECK)

#define CHECK_EQ(expression,expectation) \
    test_assert((expression)==(expectation),__LINE__,#expression "==" #expectation, CHECK)

// ASSERT_* macro performs expression result checking
// current test will be interrupted on failure
#define ASSERT_TRUE(expression) \
    do { test_assert(!!(expression),__LINE__,#expression, ASSERT); return; } while(0)

#define ASSERT_FALSE(expression) \
    do { test_assert(!(expression),__LINE__,#expression, ASSERT); return; } while(0)

#define ASSERT_EQ(expression,expectation) \
    do { test_assert((expression)==(expectation),__LINE__,#expression "==" #expectation, ASSERT); return; } while(0)

// FATAL_* macro performs expression result checking
// current test program will be interrupted on failure
#define FATAL_TRUE(expression) \
    test_assert(!!(expression),__LINE__,#expression, FATAL)

#define FATAL_FALSE(expression) \
    test_assert(!(expression),__LINE__,#expression, FATAL)

#define FATAL_EQ(expression,expectation) \
    test_assert((expression)==(expectation),__LINE__,#expression "==" #expectation, FATAL)

void test_setup(test_function_t setup);
void test_teardown(test_function_t teardown);

void test_register(test_function_t test);

int test_run(void);

#endif // AOC_TESTS_H
