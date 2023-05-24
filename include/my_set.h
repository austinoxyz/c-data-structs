#include "util/includestd.h"
#include "util/tokenpaste.h"

#ifndef _SET_MACROS__
#define _SET_MACROS__

#define SET_BLOCK_SIZE (32)
#define INITIAL_SET_CAP (SET_BLOCK_SIZE)
#define NEXT_CAPACITY_UP(curr) ((SET_BLOCK_SIZE) * (((curr) / (SET_BLOCK_SIZE)) + 1))
#define MAX_SET_SIZE ((sizeof(K_TYPE) + sizeof(V_TYPE)) * (1028 * 1028))

#endif // _SET_MACROS__

#define set_node(K) TOKENPASTE2(K, _set_node)
struct set_node(K_TYPE) {
    K_TYPE key;
    struct set_node(K_TYPE) *next;
};
typedef struct set_node(K_TYPE) set_node(K_TYPE);

#define set(K) TOKENPASTE2(K, _set)
struct set(K_TYPE) {
    size_t size;
    size_t capacity;

    set_node(K_TYPE) **buckets;

    void (*key_init)(K_TYPE *_key);
    void (*key_free)(K_TYPE *_key);
    void (*key_copy)(K_TYPE *lhs, K_TYPE rhs);
    bool (*key_equals)(K_TYPE lhs, K_TYPE rhs);
    bool (*key_compare)(K_TYPE *lhs, K_TYPE *rhs);
    unsigned long (*key_hash)(K_TYPE key);

    K_TYPE *null_key;
};
typedef struct set(K_TYPE) set(K_TYPE);

#define set_node_init(K) TOKENPASTE2(K, _set_node_init)
void set_node_init(K_TYPE) (
        set(K_TYPE) *_set, 
        set_node(K_TYPE) *node, 
        K_TYPE _key);

#define set_node_free(K) TOKENPASTE2(K, _set_node_free)
void set_node_free(K_TYPE) (
        set(K_TYPE) *_set, 
        set_node(K_TYPE) *node);

#define set_node_free_no_recurse(K) TOKENPASTE2(K, _set_node_free_no_recurse)
void set_node_free_no_recurse(K_TYPE) (
        set(K_TYPE) *_set, 
        set_node(K_TYPE) *node);

#define set_init(K) TOKENPASTE2(K, _set_init)
bool set_init(K_TYPE) (set(K_TYPE) *_set);

#define set_free(K) TOKENPASTE2(K, _set_free)
void set_free(K_TYPE) (set(K_TYPE) *_set);

#define set_insert(K) TOKENPASTE2(K, _set_insert)
bool set_insert(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key);

#define set_contains(K) TOKENPASTE2(K, _set_contains)
bool set_contains(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key);

#define set_count(K) TOKENPASTE2(K, _set_count)
size_t set_count(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key);

#define set_erase(K) TOKENPASTE2(K, _set_erase)
bool set_erase(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key);

#define _key_init(set, keyptr) \
    if ((set)->key_init)             \
        (set)->key_init((keyptr));

#define _key_free(set, keyptr)     \
    if ((set)->key_free)           \
        (set)->key_free((keyptr));

#define _key_copy(set, keyptr, _key)       \
    if ((set)->key_copy)                   \
        (set)->key_copy((keyptr), (_key)); \
    else                                   \
        *(keyptr) = (_key);

#define _key_equals(set, lhs, rhs) ((set)->key_equals((lhs), (rhs)))
#define _key_compare(set, lhs, rhs) ((set)->key_compare((lhs), (rhs)))

void set_node_init(K_TYPE) (
        set(K_TYPE) *_set,
        set_node(K_TYPE) *node, 
        K_TYPE _key
) {
    _key_init(_set, &node->key)
    _key_copy(_set, &node->key, _key)
    node->next = NULL;
}

void set_node_free(K_TYPE) (
        set(K_TYPE) *_set, 
        set_node(K_TYPE) *node
) {
    if (node->next) 
        set_node_free(K_TYPE)(_set, node->next);
    _key_free(_set, &node->key)
}

void set_node_free_no_recurse(K_TYPE) (
        set(K_TYPE) *_set, 
        set_node(K_TYPE) *node) {
    _key_free(_set, &node->key)
}

bool set_init(K_TYPE) (set(K_TYPE) *_set) 
{
    _set->size = 0;
    _set->capacity = INITIAL_SET_CAP;

    _set->buckets = (set_node(K_TYPE) **) malloc(_set->capacity * sizeof(set_node(K_TYPE) *));
    if (!_set->buckets) return false;
    for (size_t i = 0; i < _set->capacity; ++i) _set->buckets[i] = NULL;

    // These must be set after init
    _set->key_init    = NULL;
    _set->key_free    = NULL;
    _set->key_copy    = NULL;
    _set->key_equals  = NULL;
    _set->key_compare = NULL;
    _set->key_hash    = NULL; 

    return true;
}

void set_free(K_TYPE) (set(K_TYPE) *_set) {
    if (_set->buckets) {
        for (size_t bucket = 0; bucket < _set->capacity; ++bucket)
            if (_set->buckets[bucket])
                set_node_free(K_TYPE)(_set, _set->buckets[bucket]);
        free(_set->buckets);
    }
}

bool set_insert(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key) 
{
    const unsigned long bucket = _set->key_hash(_key) % _set->capacity;
    if (!_set->buckets[bucket]) {
        _set->buckets[bucket] = (set_node(K_TYPE) *) malloc(sizeof(set_node(K_TYPE)));
        set_node_init(K_TYPE)(_set, _set->buckets[bucket], _key);
        _set->size++;
        return true;
    } 

    set_node(K_TYPE) **bucketptrptr = &(_set->buckets[bucket]);
    do {
        if (_key_equals(_set, (*bucketptrptr)->key, _key)) return true;
    } while ( *(bucketptrptr = &((*bucketptrptr)->next)) );

    *bucketptrptr = (set_node(K_TYPE) *) malloc(sizeof(set_node(K_TYPE)));
    set_node_init(K_TYPE)(_set, *bucketptrptr, _key);
    if (!(*bucketptrptr)) return false;
    else return true;
}

size_t set_count(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key) 
{
    const unsigned long bucket = _set->key_hash(_key) % _set->capacity;
    if (!_set->buckets[bucket]) return 0;

    set_node(K_TYPE) *bucketptr = _set->buckets[bucket];
    do {
        if (_key_equals(_set, bucketptr->key, _key)) return 1;
    } while ((bucketptr = bucketptr->next));

    return 0;
}

bool set_contains(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key) 
{
    return set_count(K_TYPE)(_set, _key);
}

bool set_erase(K_TYPE) (set(K_TYPE) *_set, K_TYPE _key)
{
    const unsigned long bucket = _set->key_hash(_key) % _set->capacity;

    if (!_set->buckets[bucket]) return false;

    set_node(K_TYPE) **bucketptrptr = &_set->buckets[bucket];
    do {
        if (_key_equals(_set, (*bucketptrptr)->key, _key)) {
            if ((*bucketptrptr)->next) {
                set_node(K_TYPE) *temp = (*bucketptrptr)->next;
                set_node_free_no_recurse(K_TYPE)(_set, *bucketptrptr);
                *bucketptrptr = temp;
            } else {
                set_node_free(K_TYPE)(_set, *bucketptrptr);
                *bucketptrptr = NULL;
            }
            return true;
        }
    }
    while ( *(bucketptrptr = &((*bucketptrptr)->next)) );

    return false;
}

#undef _key_init
#undef _key_free
#undef _key_copy
#undef _key_compare
#undef _key_equals