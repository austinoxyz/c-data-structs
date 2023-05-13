#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifndef _VEC_MACROS__
#define _VEC_MACROS__

#define VEC_BLOCK_SIZE (32)
#define NEXT_CAPACITY_UP(curr) ((VEC_BLOCK_SIZE) * (((curr) / (VEC_BLOCK_SIZE)) + 1))
#define INITIAL_VEC_CAP (VEC_BLOCK_SIZE)

#define __TOKENPASTE2(x, y) _TOKENPASTE2(x, y)
#define _TOKENPASTE2(x, y) TOKENPASTE2(x, y)
#define TOKENPASTE2(x, y) x ## y

#define __TOKENPASTE3(x, y, z) _TOKENPASTE3(x, y, z)
#define _TOKENPASTE3(x, y, z) TOKENPASTE3(x, y, z)
#define TOKENPASTE3(x, y, z) x ## y ## z

#define __TOKENPASTE4(a, b, c, d) _TOKENPASTE4(a, b, c, d)
#define _TOKENPASTE4(a, b, c, d) TOKENPASTE4(a, b, c, d)
#define TOKENPASTE4(a, b, c, d) a ## b ## c ## d

// If only you could create macros with macros using c preprocessor
//
// #define _name(name) name
// #define define_vector_function(name) #define TOKENPASTE2(vector_, name)(T) __TOKENPASTE2(T, _TOKENPASTE2(_vector_, _name(name)))
// define_vector_function(push_back)

#endif // _VEC_MACROS__

#define vector(T) TOKENPASTE2(T, _vector)
struct vector(TYPE) {
    size_t size;
    size_t capacity;
    TYPE *buffer;

    void (*ptr_free)(TYPE*);

    TYPE null_elem;
};

typedef struct vector(TYPE) vector(TYPE);

#define vector_resize(T) TOKENPASTE2(T, _vector_resize)
void vector_resize(TYPE) (vector(TYPE) *v, const size_t new_cap);

#define vector_at(T) TOKENPASTE2(T, _vector_at)
TYPE vector_at(TYPE) (vector(TYPE) *v, const size_t idx);

#define vector_set_null_elem(T) TOKENPASTE2(T, _vector_set_null_elem)
void vector_set_null_elem(TYPE) (vector(TYPE) *v, TYPE _null_elem);

#define vector_set_free(T) TOKENPASTE2(T, _vector_set_free)
void vector_set_free(TYPE) (vector(TYPE) *v, void (*func)(TYPE*));

#define vector_free(T) TOKENPASTE2(T, _vector_free)
void vector_free(TYPE) (vector(TYPE) *v);

#define vector_init(T) TOKENPASTE2(T, _vector_init)
bool vector_init(TYPE) (vector(TYPE) *v);

#define vector_init_from(T) TOKENPASTE2(T, _vector_init_from)
bool vector_init_from(TYPE) (vector(TYPE) *v, TYPE *buf, const size_t size);

#define vector_push_back(T) TOKENPASTE2(T, _vector_push_back)
void vector_push_back(TYPE) (vector(TYPE) *v, TYPE element);

#define vector_pop_back(T) TOKENPASTE2(T, _vector_pop_back)
TYPE vector_pop_back(TYPE) (vector(TYPE) *v);

#define vector_init_with_cap(T) TOKENPASTE2(T, _vector_init_with_cap)
bool vector_init_with_cap(TYPE) (vector(TYPE) *v, const size_t cap);

#define vector_remove(T) TOKENPASTE2(T, _vector_remove)
TYPE vector_remove(TYPE) (vector(TYPE) *v, const size_t start);

// returns an array of TYPE and is the duty of user to free this
#define vector_remove_range(T) TOKENPASTE2(T, _vector_remove_range)
TYPE* vector_remove_range(TYPE) (vector(TYPE) *v, const size_t start, const size_t end);

#define vector_find(T) TOKENPASTE2(T, _vector_find)
int vector_find(TYPE) (vector(TYPE) *v, TYPE target, bool (*equals)(TYPE lhs, TYPE rhs));

TYPE vector_at(TYPE) (vector(TYPE) *v, const size_t idx) {
    return (idx >= v->size) ? v->null_elem : v->buffer[idx];
}

void vector_set_null_elem(TYPE) (vector(TYPE) *v, TYPE _null_elem) {
    v->null_elem = _null_elem;
}

void vector_set_free(TYPE) (vector(TYPE) *v, void (*func)(TYPE*)) {
    v->ptr_free = func;
}

void vector_free(TYPE) (vector(TYPE) *v) {
    if (v != NULL) {
        if (v->ptr_free != NULL) {
            for (size_t i = 0; i < v->size; ++i) {
                v->ptr_free(&v->buffer[i]);
            }
        }
        free(v->buffer);
    }
}

bool vector_init(TYPE) (vector(TYPE) *v) {
    return vector_init_with_cap(TYPE)(v, INITIAL_VEC_CAP);
}

bool vector_init_with_cap(TYPE) (vector(TYPE) *v, const size_t cap) {
    v->size = 0;
    v->capacity = cap;
    v->buffer = (TYPE *) malloc(cap * sizeof(TYPE));
    v->ptr_free = NULL;
    return v->buffer != NULL;
}

bool vector_init_from(TYPE) (vector(TYPE) *v, TYPE *buf, const size_t size) {
    vector(TYPE) *ret = (vector(TYPE) *) malloc(sizeof(vector(TYPE)));
    if (ret == NULL) {
        printf("ret is null.\n");
        return false;
    }
    v->size = size;
    v->capacity = NEXT_CAPACITY_UP(size);
    v->buffer = (TYPE *) malloc(v->capacity * sizeof(TYPE));
    if (v->buffer == NULL) {
        printf("buffer is null.\n");
        return false;
    }
    memcpy(v->buffer, buf, size * sizeof(TYPE));
    return true;
}

void vector_push_back(TYPE) (vector(TYPE) *v, TYPE element) {
    v->buffer[v->size++] = element;
}

TYPE vector_pop_back(TYPE) (vector(TYPE) *v) {
    return (v->size == 0) ? v->null_elem : v->buffer[--v->size];
}

TYPE vector_remove(TYPE) (vector(TYPE) *v, const size_t pos) {
    if (pos >= v->size)
        return v->null_elem;
    TYPE ret = v->buffer[pos];
    if (pos != --v->size)
        memmove(&v->buffer[pos], &v->buffer[pos + 1], (v->size - pos) * sizeof(TYPE));
    return ret;
}

int vector_find(TYPE) (vector(TYPE) *v, TYPE target, bool (*equals)(TYPE lhs, TYPE rhs)) {
    for (size_t i = 0; i < v->size; ++i)
        if (equals(target, v->buffer[i])) return i;
    return -1;
}

// v->capacity will remain unchanged if unsuccessful
void vector_resize(TYPE) (vector(TYPE) *v, const size_t new_cap) {
    if (v->buffer == NULL)
        return;
    v->buffer = (TYPE *) realloc(v->buffer, new_cap * sizeof(TYPE));
    if (v->buffer != NULL)
        v->capacity = new_cap;
}

TYPE* vector_remove_range(TYPE) (vector(TYPE) *v, const size_t start, const size_t end) {
    if (start >= end || start >= v->size || end > v->size || start == end)
        return NULL;
    const size_t range = end - start;
    TYPE *ret = (TYPE *) malloc(range * sizeof(TYPE));
    memcpy(ret, &v->buffer[start], range * sizeof(TYPE));
    if (end < v->size) {
        printf("Moving %zu. start=%zu\n", range, start);
        memmove(&v->buffer[start], &v->buffer[end], (v->size - end) * sizeof(TYPE));
    }
    v->size -= range;
    return ret;
}

