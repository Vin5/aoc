#include <string.h>
#include "tests.h"
#include "active.h"

static char message[] = "Hello world!";

static void reverser(void) {
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
    active_t* object = active_new();
    active_send(object, reverser);
    active_destroy(&object);
    CHECK_EQ(strcmp(message, "!dlrow olleH"), 0);
}

int main(int argc, char* argv[]) {
    test_register(integration_test);
    return test_run();
}
