#include <assert.h>

#define K_TYPE int
#include "my_set.h"
#undef K_TYPE

unsigned long int_hash(const int n) {
    return (unsigned long) n;
}

int main(void) {
    set(int) _set;
    set_init(int)(&_set);
    _set.key_hash = int_hash;

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
}
