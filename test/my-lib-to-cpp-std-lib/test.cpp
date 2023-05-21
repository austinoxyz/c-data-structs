#include "my_string.h"

#define TYPE string
#include "my_vector.h"
#undef TYPE

int main(void) {
    vector(string) v;
    for (int i = 0; i < 10; ++i) {
        string s;
        string_init_from_cstr(&s, rand_cstr(15));
        vector_push_back(string)(&v, s);
        string_free(&s)
    }

    vector_free(string)(&v);
}
