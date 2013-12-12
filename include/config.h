#ifndef AOC_CONFIG_H
#define AOC_CONFIG_H

#define HAVE_STDBOOL
#define HAVE_PTHREAD

#ifdef _WIN32
    #define AOC_EXPORT __declspec(dllexport)
#else
    #define AOC_EXPORT
#endif

#endif // AOC_CONFIG_H
