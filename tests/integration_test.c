#include <string.h>

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

int main(int argc, char* argv[]) {
    active_t* object = active_new();
    active_send(object, reverser);
    active_destroy(&object);
    return strcmp(message, "!dlrow olleH");
}
