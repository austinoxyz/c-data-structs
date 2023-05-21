#include "test.h"

#include <assert.h>
#include <time.h>

#define K_TYPE int
#include "my_set.h"
#undef K_TYPE

unsigned long int_hash(const int n) {
    return (unsigned long) n;
}

bool int_equals(int lhs, int rhs) { return lhs == rhs; }

#define _new_int_set(name)        \
    set(int) name;                \
    set_init(int)(&name);         \
    name.key_hash = int_hash;     \
    name.key_equals = int_equals; \

void simple_test() {
    _new_int_set(_set)

    bool success;

    success = set_insert(int)(&_set, 4);
    assert(success);

    success = set_insert(int)(&_set, 36);
    assert(success);

    success = set_insert(int)(&_set, 68);
    assert(success);

    success = set_insert(int)(&_set, 4);
    assert(success);

    success = set_contains(int)(&_set, 4);
    assert(success);

    success = set_erase(int)(&_set, 36);
    assert(success);

    success = set_contains(int)(&_set, 36);
    assert(!success);

    success = set_insert(int)(&_set, 100);
    assert(success);

    set_free(int)(&_set);
}

void stress_test() {
    _new_int_set(_set)

    bool success;
    for (int i = 0; i < 100000; ++i) {
        success = set_insert(int)(&_set, i);
        assert(success);
    }

    success = set_contains(int)(&_set, 10000);
    assert(success);
    success = set_contains(int)(&_set, 99999);
    assert(success);
    success = set_contains(int)(&_set, 100000);
    assert(!success);
}

int main(void) {
    time_function(simple_test);
    time_function(stress_test);
}
