#include <string.h>
#include "tests.h"
#include "active.h"
#include "boolean.h"

static bool execution_finished = false;

static void callback() {
    execution_finished = true;
}

static void reverser(void* params) {
    char* message = (char*) params;
    size_t len = strlen(message);
    int i = 0;
    char temp;
    for(; i < len / 2; i++) {
        temp = message[len - i - 1];
        message[len - i - 1] = message[i];
        message[i] = temp;
    }
}

void integration_test() {
    char message[] = "Hello world!";
    active_t* object = active_new();
    active_send(object, reverser, message, callback);
    active_destroy(&object);
    CHECK_EQ(strcmp(message, "!dlrow olleH"), 0);
    CHECK_TRUE(execution_finished);
}

int main(int argc, char* argv[]) {
    test_register(integration_test);
    return test_run();
}
