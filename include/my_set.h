#include "util/includestd.h"
#include "util/tokenpaste.h"

#ifndef _SET_MACROS__
#define _SET_MACROS__
  #define SET_BLOCK_SIZE (16)
  #define INITIAL_SET_CAP (SET_BLOCK_SIZE)
  #define NEXT_CAPACITY_UP(curr) ((SET_BLOCK_SIZE) * (((curr) / (SET_BLOCK_SIZE)) + 1))
  #define SET_MAX_N_BUCKETS ((SET_BLOCK_SIZE) * 64)
#endif // _SET_MACROS__

#define set_node(K) PASTE2(K, _set_node)
typedef struct set_node(TYPE) {
    TYPE key;
    struct set_node(TYPE) *next;
} set_node(TYPE);

#define init_func(K)    PASTE3(set_key_,K,_init_func)
#define free_func(K)    PASTE3(set_key_,K,_free_func)
#define copy_func(K)    PASTE3(set_key_,K,_copy_func)
#define equals_func(K)  PASTE3(set_key_,K,_equals_func)
#define compare_func(K) PASTE3(set_key_,K,_compare_func)
#define hash_func(K)    PASTE3(set_key_,K,_hash_func)

typedef void (*init_func(TYPE))(TYPE *);
typedef void (*free_func(TYPE))(TYPE *);
typedef void (*copy_func(TYPE))(TYPE *, TYPE);
typedef bool (*equals_func(TYPE))(TYPE, TYPE);
typedef int (*compare_func(TYPE))(TYPE, TYPE);
typedef unsigned long (*hash_func(TYPE))(TYPE);

#define set(K) PASTE2(K, _set)
typedef struct set(TYPE) 
{
    size_t size;
    size_t capacity;
    set_node(TYPE) **buckets;

    init_func(TYPE)    key_init;
    free_func(TYPE)    key_free;
    copy_func(TYPE)    key_copy;
    equals_func(TYPE)  key_equals;
    compare_func(TYPE) key_compare;
    hash_func(TYPE)    key_hash;
} set(TYPE);

#define set_node_init(K) PASTE2(K, _set_node_init)
void set_node_init(TYPE) (
        set(TYPE) *_set, 
        set_node(TYPE) *node, 
        TYPE _key);

#define set_node_free(K) PASTE2(K, _set_node_free)
void set_node_free(TYPE) (
        set(TYPE) *_set, 
        set_node(TYPE) *node);

#define set_node_free_no_recurse(K) PASTE2(K, _set_node_free_no_recurse)
void set_node_free_no_recurse(TYPE) (
        set(TYPE) *_set, 
        set_node(TYPE) *node);

#define set_init(K) PASTE2(K, _set_init)
void set_init(TYPE) (set(TYPE) *_set);

#define new_set(K) PASTE3(new_,K,_set)
set(TYPE) new_set(TYPE)(init_func(TYPE), free_func(TYPE), copy_func(TYPE), equals_func(TYPE), compare_func(TYPE), hash_func(TYPE));

#define set_init_with_capacity(K) PASTE2(K, _set_init_with_capacity)
void set_init_with_capacity(TYPE) (set(TYPE) *_set, const size_t cap);

#define set_free(K) PASTE2(K, _set_free)
void set_free(TYPE) (set(TYPE) *_set);

#define set_resize(K) PASTE2(K, _set_resize)
void set_resize(TYPE) (set(TYPE) *_set, const size_t new_cap);

#define set_insert(K) PASTE2(K, _set_insert)
bool set_insert(TYPE) (set(TYPE) *_set, TYPE _key);

#define set_key_exists(K) PASTE2(K, _set_key_exists)
bool set_key_exists(TYPE) (set(TYPE) *_set, TYPE _key);

#define set_erase(K) PASTE2(K, _set_erase)
bool set_erase(TYPE) (set(TYPE) *_set, TYPE _key);

#define _key_init(set, keyptr) \
    if ((set)->key_init)       \
        (set)->key_init((keyptr));

#define _key_free(set, keyptr) \
    if ((set)->key_free)       \
        (set)->key_free((keyptr));

#define _key_copy(set, keyptr, _key)       \
    if ((set)->key_copy)                   \
        (set)->key_copy((keyptr), (_key)); \
    else                                   \
        *(keyptr) = (_key);

#define _key_equals(set, lhs, rhs) ((set)->key_equals((lhs), (rhs)))
#define _key_compare(set, lhs, rhs) ((set)->key_compare((lhs), (rhs)))

void set_node_init(TYPE) (
        set(TYPE) *_set,
        set_node(TYPE) *node, 
        TYPE _key
) {
    _key_init(_set, &node->key)
    _key_copy(_set, &node->key, _key)
    node->next = NULL;
}

void set_node_free(TYPE) (
        set(TYPE) *_set, 
        set_node(TYPE) *node
) {
    if (node->next) {
        set_node_free(TYPE)(_set, node->next);
        node->next = NULL;
    }
    _key_free(_set, &node->key)
}

void set_node_free_no_recurse(TYPE) (
        set(TYPE) *_set, 
        set_node(TYPE) *node) {
    _key_free(_set, &node->key)
}

void set_init(TYPE) (set(TYPE) *_set) 
{
    set_init_with_capacity(TYPE)(_set, INITIAL_SET_CAP);
}

void set_init_with_capacity(TYPE) (set(TYPE) *_set, const size_t cap) 
{
    _set->size = 0;
    _set->capacity = cap;

    _set->buckets = (set_node(TYPE) **) malloc(_set->capacity * sizeof(set_node(TYPE) *));
    for (size_t i = 0; i < _set->capacity; ++i) _set->buckets[i] = NULL;

    // These must be set after init
    _set->key_init    = NULL;
    _set->key_free    = NULL;
    _set->key_copy    = NULL;
    _set->key_equals  = NULL;
    _set->key_compare = NULL;
    _set->key_hash    = NULL; 
}
void set_free(TYPE) (set(TYPE) *_set) {
    if (_set->buckets) {
        for (size_t bucket = 0; bucket < _set->capacity; ++bucket)
            if (_set->buckets[bucket]) {
                set_node_free(TYPE)(_set, _set->buckets[bucket]);
                _set->buckets[bucket] = NULL;
            }
        free(_set->buckets);
        _set->buckets = NULL;
    }
}

void set_resize(TYPE) (set(TYPE) *_set, const size_t _n_buckets) 
{
    if (_set->capacity == _n_buckets || _n_buckets > SET_MAX_N_BUCKETS) 
        return;

    set(TYPE) *new_set = (set(TYPE) *) malloc(sizeof(set(TYPE)));
    if ( !new_set ) return;

    set_init_with_capacity(TYPE)(new_set, _n_buckets);
    if ( !(new_set->buckets) ) return;

    new_set->key_init    = _set->key_init;
    new_set->key_free    = _set->key_free;
    new_set->key_copy    = _set->key_copy;
    new_set->key_equals  = _set->key_equals;
    new_set->key_compare = _set->key_compare;
    new_set->key_hash    = _set->key_hash;

    set_node(TYPE) **bucketptrptr;
    for (size_t bucket = 0; bucket < _set->capacity; ++bucket) {
        bucketptrptr = &_set->buckets[bucket];
        while ( *bucketptrptr ) {
            set_insert(TYPE)(new_set, (*bucketptrptr)->key);
            bucketptrptr = &((*bucketptrptr)->next);
        }
    }

    set_free(TYPE)(_set);

    _set->size = new_set->size;
    _set->capacity = new_set->capacity;
    _set->buckets = new_set->buckets;

    free(new_set);
}

bool set_insert(TYPE) (set(TYPE) *_set, TYPE _key) 
{
    const unsigned long bucket = _set->key_hash(_key) % _set->capacity;
    if (!_set->buckets[bucket]) {
        _set->buckets[bucket] = (set_node(TYPE) *) malloc(sizeof(set_node(TYPE)));
        if ( !_set->buckets[bucket] ) return false;
        set_node_init(TYPE)(_set, _set->buckets[bucket], _key);
        _set->size++;
        return true;
    } 

    set_node(TYPE) **bucketptrptr = &(_set->buckets[bucket]);
    do {
        if (_key_equals(_set, (*bucketptrptr)->key, _key)) return true;
    } while ( *(bucketptrptr = &((*bucketptrptr)->next)) );

    *bucketptrptr = (set_node(TYPE) *) malloc(sizeof(set_node(TYPE)));
    set_node_init(TYPE)(_set, *bucketptrptr, _key);
    if ( !(*bucketptrptr) ) return false;
    _set->size += 1;
    return true;
}

bool set_key_exists(TYPE) (set(TYPE) *_set, TYPE _key) 
{
    const unsigned long bucket = _set->key_hash(_key) % _set->capacity;
    if (!_set->buckets[bucket]) return false;

    set_node(TYPE) *bucketptr = _set->buckets[bucket];
    do {
        if (_key_equals(_set, bucketptr->key, _key)) return true;
    } while ( (bucketptr = bucketptr->next) );

    return false;
}

bool set_erase(TYPE) (set(TYPE) *_set, TYPE _key)
{
    const unsigned long bucket = _set->key_hash(_key) % _set->capacity;

    if (!_set->buckets[bucket]) return false;

    set_node(TYPE) **bucketptrptr = &_set->buckets[bucket];
    do {
        if (_key_equals(_set, (*bucketptrptr)->key, _key)) {
            if ((*bucketptrptr)->next) {
                set_node(TYPE) *temp = (*bucketptrptr)->next;
                set_node_free_no_recurse(TYPE)(_set, *bucketptrptr);
                *bucketptrptr = temp;
            } else {
                set_node_free(TYPE)(_set, *bucketptrptr);
                *bucketptrptr = NULL;
            }
            return true;
        }
    } while ( *(bucketptrptr = &((*bucketptrptr)->next)) );

    return false;
}

#undef _key_init
#undef _key_free
#undef _key_copy
#undef _key_compare
#undef _key_equals
