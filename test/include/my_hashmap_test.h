#include "util/test.h"

#include "my_set.h"

// vec_data requires atleast 32 elements. (2 * INITIAL_VEC_CAP)
#define set_data(K, V) PASTE5(K, _, V, _, _set_data)
#define set_data_size(T) PASTE5(K, _, V, _, _set_data_size)

#define set_data_size(K, V) PASTE4(K, _, V, _, pair)
typedef struct key_val_pair(K_TYPE, V_TYPE) {
    K_TYPE key;
    V_TYPE val;
} key_val_pair(K_TYPE, V_TYPE);

static TYPE *set_data(K_TYPE, V_TYPE) = NULL;
static size_t set_data_size(K_TYPE, V_TYPE) = 0;

#define key_init_func(V) PASTE3(_, V, _init_func)
#define key_free_func(V) PASTE3(_, V, _free_func)
#define key_copy_func(V) PASTE3(_, V, _copy_func)
#define key_equals_func(V) PASTE3(_, V, _equals_func)
#define key_compare_func(V) PASTE3(_, V, _compare_func)
#define hash_func(V) PASTE3(_, V, _hash_func)

static void (*key_init_func(K_TYPE))(K_TYPE *_key)             = NULL;
static void (*key_free_func(K_TYPE))(K_TYPE  *_key)            = NULL;
static void (*key_copy_func(K_TYPE))(K_TYPE *lhs, K_TYPE rhs)  = NULL;
static bool (*key_equals_func(K_TYPE))(K_TYPE lhs, K_TYPE rhs) = NULL;
static int (*key_compare_func(K_TYPE))(K_TYPE lhs, K_TYPE rhs) = NULL;
static unsigned long (*hash_func(K_TYPE))(K_TYPE lhs)          = NULL;

#define val_init_func(V) PASTE3(_, V, _init_func)
#define val_free_func(V) PASTE3(_, V, _free_func)
#define val_copy_func(V) PASTE3(_, V, _copy_func)

static void (*val_init_func(V_TYPE))(V_TYPE *_val)            = NULL;
static void (*val_free_func(V_TYPE))(V_TYPE  *_val)           = NULL;
static void (*val_copy_func(V_TYPE))(V_TYPE *lhs, V_TYPE rhs) = NULL;

#define map_set_data(T, data, size) \
{                                   \
    map_data(T) = (data);           \
    map_data_size(T) = (size);      \
}

#define prepare_empty_map(name) \
    map(TYPE) name;                              \
    map_init(TYPE)(&(name));                     \
    {                                            \
        name.key_init = init_func(K_TYPE);       \
        name.key_free = free_func(K_TYPE);       \
        name.key_copy = copy_func(K_TYPE);       \
        name.key_equals = equals_func(K_TYPE);   \
        name.key_compare = compare_func(K_TYPE); \
        name.key_hash = hash_func(K_TYPE);       \
                                                 \
        name.value_init = val_init_func(V_TYPE); \
    }

#define prepare_filled_map(name) \
