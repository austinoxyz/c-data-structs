#define TYPE int
#include "my_vector.h"
#undef TYPE

void free_int_vector(vector(int) *v) {
    free(v->buffer);
}

typedef struct A {
    char *buffer;
} A;

#define TYPE A
#include "my_vector.h"
#undef TYPE

void init_A(A *a, size_t offset) {
    a->buffer = (char *)malloc(26 * sizeof(char));
    for (int i = 0; i < 26; ++i)
        a->buffer[i] = 'a' + i + offset;
    a->buffer[26] = '\0';
}

void free_A(A *a) {
    if (a->buffer != NULL) 
        free(a->buffer);
}

void free_A_vector(vector(A) *v) {
    if (v != NULL)
        for (size_t i = 0; i < v->size; ++i)
            free_A(&v->buffer[i]);
}

bool equals_A(A lhs, A rhs) {
    return !strcmp(lhs.buffer, rhs.buffer);
}

#define RUN_TEST(x) do {              \
    printf("Running %s...\n", #x);    \
    x();                              \
    printf("Completed %s.\n\n", #x);  \
} while(0)

void print_int_vector(vector(int) *v) {
    for (size_t i = 0; i < v->size; ++i) {
        const int n = vector_at(int)(v, i);
        printf("%d, ", n);
    }
    printf("\n");
}

void print_A(A *a) {
    printf("%s\n", a->buffer);
}

void print_A_vector(vector(A) *v) {
    for (size_t i = 0; i < v->size; ++i) {
        print_A(&v->buffer[i]);
    }
    printf("\n");
}

void test_int_vector() {
    bool success;
    vector(int) v;
    success = vector_init(int)(&v);
    if (!success) exit(1);
    else printf("int_vector initialized.\n");

    for (int i = 0; i < 10; ++i) {
        vector_push_back(int)(&v, i);
    }

    const size_t start = 2, end = v.size, range = end - start;
    int *remove_buffer = vector_remove_range(int)(&v, start, end);

    print_int_vector(&v);

    vector(int) removed_v;
    success = vector_init_from(int)(&removed_v, remove_buffer, range);
    if (!success) exit(1);
    else printf("removed_v int_vector initialized.\n");
    print_int_vector(&removed_v);

    vector_free(int)(&v);
}

const char *alphabet = "abcdefghijklmnopqrstuvwxyz";

void test_A_vector() {
    vector(A) v;
    bool success = vector_init(A)(&v);
    vector_set_free(A)(&v, free_A);
    if (!success) exit(1);
    else printf("A_vector initialized.\n");
    printf("offsetof(A, buffer)=%zu\n", offsetof(A, buffer));

    for (int i = 0; i < 5; ++i) {
        A a; 
        init_A(&a, i);
        vector_push_back(A)(&v, a);
    }

    print_A_vector(&v);

    const size_t start = 1, end = 1, range = end - start;
    A *remove_buffer = vector_remove_range(A)(&v, start, end);

    vector(A) removed_v;
    success = vector_init_from(A)(&removed_v, remove_buffer, range);
    if (!success) exit(1);
    else printf("removed_v int_vector initialized.\n");
    vector_set_free(A)(&v, free_A);
    print_A_vector(&removed_v);

    A alphabet_a;
    alphabet_a.buffer = (char *) malloc (26 * sizeof(char));
    memcpy(alphabet_a.buffer, alphabet, 26 * sizeof(char));

    const int res = vector_find(A)(&v, alphabet_a, equals_A);
    printf("Find result: %d\n", res);

    vector_free(A)(&v);
    vector_free(A)(&removed_v);
    free_A(&alphabet_a);
    printf("A_vector freed.\n");
}

#if 1
int main(void) {
    RUN_TEST(test_int_vector);
    RUN_TEST(test_A_vector);
}
#endif // main
