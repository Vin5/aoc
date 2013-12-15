aoc
===

Active object pattern implementation in pure C

aoc is a simple threading framework allowing to execute some  
tasks in a backgound thread associated with an active object.

Example:
```cpp
#include <active.h>

void print(void* data) {
	const char* message = (const char*) data;
	printf("%s", message);
}

void some_func(aoc_active_t* background_logger) {
	// do smth...    

	// next call returns immediately
        aoc_active_send(background_logger, print, (void*)"Message to log", NULL); // message will be logged in background

	// do smth...
}

int main(int argc, char* argv[]) {
        aoc_active_t* worker = aoc_active_new();
	// ... 
	some_func(worker);

	// ...
        aoc_active_destroy(&worker); // waits for all async tasks is finished
	return 0;
}
```
More examples of usage you can find in tests/integration_test.c.

Background thread starts with an active object creation and lives while  
the associated active object is alive.

Asynchronous task finish can be caught with callback function which  
is the last parameter of 'active_send' function.

If you need to implement a tricky synchronization mechanism the library  
provides some low level syncronization primitives, such as:
- mutex
- condition variable
- blocking queue  
examples of their usages you can find in 'tests' directory.

For example you might want to wait for some of your async tasks is finished.  
In this case you can use callback function and condition variable in  
the following manner:
```cpp
#include <active.h>
#include <condition.h>

static aoc_condition_t* finish = NULL;

int main(int argc, char* argv[]) {
        aoc_active_t* worker = aoc_active_new();
        condition = aoc_condition_new();

        aoc_active_send(worker, print, (void*)"Hello world", callback);
        aoc_condition_acquire(finish);
        aoc_condition_wait(finish);
        aoc_condition_release(finish);

        aoc_condition_destroy(finish);
        aoc_active_destroy(worker);
}

void callback(void) {
        aoc_condition_notify(finish);
}
```
### Build instructions (linux)
* * *
    $ cd aoc
    $ mkdir build && cd build
    $ cmake -DENABLE_TESTS=1 -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
    $ make
    $ ctest

### Build requirements
* * *
- CMake version >= 2.6

### Tested platforms (probably works on the other platforms as well)
* * *
Linux:  

- GCC 4.3
- GCC 4.7

Windows:  

- VS 8.0
- VS 10.0

### LICENSE
* * *
See LICENSE file in the project directory
