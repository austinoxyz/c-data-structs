#ifndef _TEST_H__
#define _TEST_H__

#include "time.h"
#include "assert.h"

#define strfy(x) #x

#define time_function(test_func)                                \
{                                                               \
    printf("Running test %s...", strfy(test_func));             \
    clock_t start = clock(), diff;                              \
    test_func();                                                \
    diff = clock() - start;                                     \
    int ms = diff * 1000 / CLOCKS_PER_SEC;                      \
    printf(" completed in %d.%d seconds.\n", ms/1000, ms%1000); \
}

#endif // _TEST_H__

