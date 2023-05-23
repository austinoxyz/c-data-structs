#include "includestd.h"
#include "tokenpaste.h"

#ifndef _VEC_MACROS__
#define _VEC_MACROS__

#define VEC_BLOCK_SIZE (16)
#define NEXT_CAPACITY_UP(curr) ((VEC_BLOCK_SIZE) * (((curr) / (VEC_BLOCK_SIZE)) + 1))
#define INITIAL_VEC_CAP (VEC_BLOCK_SIZE)

#endif // _VEC_MACROS__

#define vector(T) TOKENPASTE2(T, _vector)
struct vector(TYPE) {
    size_t size;
    size_t capacity;
    TYPE *buffer;

    void (*elem_init)(TYPE *_elem);
    void (*elem_free)(TYPE *_elem);
    void (*elem_copy)(TYPE *lhs, TYPE rhs);
    bool (*elem_equals)(TYPE lhs, TYPE rhs);
};

typedef struct vector(TYPE) vector(TYPE);

#define vector_resize(T) TOKENPASTE2(T, _vector_resize)
bool vector_resize(TYPE) (vector(TYPE) *vec, const size_t new_cap);

#define vector_at(T) TOKENPASTE2(T, _vector_at)
bool vector_at(TYPE) (vector(TYPE) *vec, const size_t idx, TYPE *_elem_retval);

#define vector_free(T) TOKENPASTE2(T, _vector_free)
void vector_free(TYPE) (vector(TYPE) *vec);

#define vector_init(T) TOKENPASTE2(T, _vector_init)
bool vector_init(TYPE) (vector(TYPE) *vec);

#define vector_init_from(T) TOKENPASTE2(T, _vector_init_from)
bool vector_init_from(TYPE) (vector(TYPE) *vec, TYPE *buf, const size_t size);

#define vector_push_back(T) TOKENPASTE2(T, _vector_push_back)
void vector_push_back(TYPE) (vector(TYPE) *vec, TYPE element);

#define vector_pop_back(T) TOKENPASTE2(T, _vector_pop_back)
bool vector_pop_back(TYPE) (vector(TYPE) *vec, TYPE *_elem_retval);

#define vector_remove(T) TOKENPASTE2(T, _vector_remove)
bool vector_remove(TYPE) (vector(TYPE) *vec, const size_t pos);

//// returns an array of TYPE and is the duty of user to free this
//#define vector_remove_range(T) TOKENPASTE2(T, _vector_remove_range)
//TYPE* vector_remove_range(TYPE) (vector(TYPE) *vec, const size_t start, const size_t end);

#define vector_find(T) TOKENPASTE2(T, _vector_find)
int vector_find(TYPE) (vector(TYPE) *vec, TYPE target);

#define _elem_init(vec, elemptr)     \
    if ((vec)->elem_init)            \
        (vec)->elem_init((elemptr));

#define _elem_init_return(vec, elemptr)     \
    ((!((vec)->elem_init) && !((vec)->elem_init((elemptr))) ) ? : false : true)

#define _elem_free(vec, elemptr)     \
    if ((vec)->elem_free)            \
        (vec)->elem_free((elemptr));

#define _elem_copy(vec, elemptr, _elem)       \
    if ((vec)->elem_copy)                     \
        (vec)->elem_copy((elemptr), (_elem)); \
    else                                      \
        *(elemptr) = (_elem);

#define _elem_equals(vec, lhs, rhs) ((vec)->elem_equals((lhs), (rhs)))

void vector_free(TYPE) (vector(TYPE) *vec) {
    if (vec != NULL) {
        if (vec->elem_free != NULL) {
            for (size_t i = 0; i < vec->size; ++i) {
                _elem_free(vec, &vec->buffer[i])
            }
        }
        free(vec->buffer);
    }
}

bool vector_init(TYPE) (vector(TYPE) *vec) {
    vec->size = 0;
    vec->capacity = INITIAL_VEC_CAP;

    vec->elem_init   = NULL;
    vec->elem_free   = NULL;
    vec->elem_copy   = NULL;
    vec->elem_equals = NULL;

    vec->buffer = (TYPE *) malloc(INITIAL_VEC_CAP * sizeof(TYPE));
    if (!(vec->buffer)) return false;

    return true;
}

bool vector_init_from(TYPE) (vector(TYPE) *vec, TYPE *_src_buffer, const size_t _size) {
    vector_init(TYPE)(vec);
    if ( !(vector_resize(TYPE)(vec, NEXT_CAPACITY_UP(_size))) ) return false;
    memcpy(vec->buffer, _src_buffer, _size * sizeof(TYPE));
    return true;
}

void vector_push_back(TYPE) (vector(TYPE) *vec, TYPE _elem) {
    if (vec->size == vec->capacity 
        && !(vector_resize(TYPE)(vec, vec->capacity + VEC_BLOCK_SIZE)) ) 
        return;
    _elem_init(vec, &vec->buffer[vec->size])
    _elem_copy(vec, &vec->buffer[vec->size], _elem)
    vec->size++;
}

bool vector_pop_back(TYPE) (vector(TYPE) *vec, TYPE *_elem_retval) {
    if (vec->size > 0) {
        *_elem_retval = vec->buffer[--vec->size];
        return true;
    }
    return false;
}

bool vector_at(TYPE) (vector(TYPE) *vec, const size_t idx, TYPE *_elem_retval) {
    if (idx >= vec->size) return false;
    _elem_copy(vec, _elem_retval, vec->buffer[idx]);
    return true;
}

bool vector_remove(TYPE) (vector(TYPE) *vec, const size_t pos) {
    if (pos >= vec->size)
        return false;
    if (pos != --vec->size)
        memmove(&vec->buffer[pos], &vec->buffer[pos + 1], (vec->size - pos) * sizeof(TYPE));
    return (bool) (vec->buffer);
}

int vector_find(TYPE) (vector(TYPE) *vec, TYPE target) {
    for (size_t i = 0; i < vec->size; ++i)
        if (_elem_equals(vec, vec->buffer[i], target)) return i;
    return -1;
}

// v->capacity will remain unchanged if unsuccessful
bool vector_resize(TYPE) (vector(TYPE) *vec, const size_t new_cap) {
    if ( !(vec->buffer) ) return false;
    vec->buffer = (TYPE *) realloc(vec->buffer, new_cap * sizeof(TYPE));
    if (vec->buffer) vec->capacity = new_cap;
    return true;
}

//TYPE* vector_remove_range(TYPE) (vector(TYPE) *vec, const size_t start, const size_t end) {
//    if (start >= end || start >= vec->size || end > vec->size || start == end)
//        return NULL;
//    const size_t range = end - start;
//    TYPE *ret = (TYPE *) malloc(range * sizeof(TYPE));
//    memcpy(ret, &vec->buffer[start], range * sizeof(TYPE));
//    if (end < vec->size) {
//        printf("Moving %zu. start=%zu\n", range, start);
//        memmove(&vec->buffer[start], &vec->buffer[end], (vec->size - end) * sizeof(TYPE));
//    }
//    v->size -= range;
//    return ret;
//}

