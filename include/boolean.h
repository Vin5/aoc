#ifndef AOC_BOOLEAN_H
#define AOC_BOOLEAN_H

#include "config.h"

#ifdef HAVE_STDBOOL
    #include <stdbool.h>
#else
    #define bool char
    #define true  1
    #define false 0
#endif

#endif // AOC_BOOLEAN_H
