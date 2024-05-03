#include "util/includestd.h"
#include "util/tokenpaste.h"

// must define K_TYPE, V_TYPE

#ifndef _MAP_MACROS__
#define _MAP_MACROS__

#define MAP_BLOCK_SIZE (32)
#define INITIAL_MAP_CAP (MAP_BLOCK_SIZE)
#define NEXT_CAPACITY_UP(curr) ((MAP_BLOCK_SIZE) * (((curr) / (MAP_BLOCK_SIZE)) + 1))
#define MAX_MAP_SIZE ((sizeof(K_TYPE) + sizeof(V_TYPE)) * (1028 * 1028))

#endif // _MAP_MACROS__

#define map_node(K, E) PASTE4(K, _, E, _map_node)
struct map_node(K_TYPE, V_TYPE) {
    K_TYPE key;
    V_TYPE value;
    struct map_node(K_TYPE, V_TYPE) *next;
};
typedef struct map_node(K_TYPE, V_TYPE) map_node(K_TYPE, V_TYPE);

#define map(K, E) PASTE4(K, _, E, _map)
struct map(K_TYPE, V_TYPE) {
    size_t size;
    size_t capacity;

    map_node(K_TYPE, V_TYPE) **buckets;

    void (*key_init)(K_TYPE *_key);
    void (*key_free)(K_TYPE *_key);
    void (*key_copy)(K_TYPE *lhs, K_TYPE rhs);
    int (*key_compare)(K_TYPE *lhs, K_TYPE *rhs);
    bool (*key_equals)(K_TYPE lhs, K_TYPE rhs);
    unsigned long (*key_hash)(K_TYPE key);

    void (*value_init)(V_TYPE *_value);
    void (*value_free)(V_TYPE *_value);
    void (*value_copy)(V_TYPE *lhs, V_TYPE rhs);
};
typedef struct map(K_TYPE, V_TYPE) map(K_TYPE, V_TYPE);

#define map_node_init(K, E) PASTE4(K, _, E, _map_node_init)
void map_node_init(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *map, 
        map_node(K_TYPE, V_TYPE) *node, 
        K_TYPE _key, 
        V_TYPE _value);

#define map_init(K, E) PASTE4(K, _, E, _map_init)
void map_init(K_TYPE, V_TYPE) (map(K_TYPE, V_TYPE) *_map);

#define map_free(K, E) PASTE4(K, _, E, _map_free)
void map_free(K_TYPE, V_TYPE) (map(K_TYPE, V_TYPE) *_map);

#define map_node_free(K, E) PASTE4(K, _, E, _map_node_free)
void 
map_node_free(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        map_node(K_TYPE, V_TYPE) *node);

#define map_node_free_no_recurse(K, E) PASTE4(K, _, E, _map_node_free_no_recurse)
void 
map_node_free_no_recurse(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        map_node(K_TYPE, V_TYPE) *node);

#define map_node_copy(K, E) PASTE4(K, _, E, _map_node_copy)
void 
map_node_copy(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        map_node(K_TYPE, V_TYPE) *lhs,
        map_node(K_TYPE, V_TYPE) *rhs);

#define map_resize(K, E) PASTE4(K, _, E, _map_resize)
bool 
map_resize(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        const size_t new_cap);

#define map_insert_or_assign(K, E) PASTE4(K, _, E, _map_insert_or_assign)
bool 
map_insert_or_assign(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key, 
        V_TYPE _value);

#define map_insert(K, E) PASTE4(K, _, E, _map_insert)
bool 
map_insert(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key, V_TYPE _value);

#define map_at(K, E) PASTE4(K, _, E, _map_at)
bool 
map_at(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key, V_TYPE *value_ret_value);

#define map_contains(K, E) PASTE4(K, _, E, _map_contains)
bool 
map_contains(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key);

#define map_count(K, E) PASTE4(K, _, E, _map_count)
size_t 
map_count(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key);

#define map_find(K, E) PASTE4(K, _, E, _map_find)
map_node(K_TYPE, V_TYPE) * 
map_find(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key);

#define map_remove(K, E) PASTE4(K, _, E, _map_remove)
bool 
map_remove(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key);

#define _key_init(map, keyptr)     \
    if ((map)->key_init)           \
        (map)->key_init((keyptr));

#define _key_free(map, keyptr)     \
    if ((map)->key_free)           \
        (map)->key_free((keyptr));

#define _key_copy(map, keyptr, _key)       \
    if ((map)->key_copy)                   \
        (map)->key_copy((keyptr), (_key)); \
    else                                   \
        *(keyptr) = (_key);

#define _key_equals(map, lhs, rhs) ((map)->key_equals((lhs), (rhs)))
#define _key_compare(map, lhs, rhs) ((map)->key_compare((lhs), (rhs)))

#define _value_init(map, valueptr)    \
    if ((map)->value_init)            \
        (map)->value_init((valueptr));

#define _value_free(map, valueptr)    \
    if ((map)->value_free)            \
        (map)->value_free((valueptr));

#define _value_copy(map, valueptr, _value)       \
    if ((map)->value_copy)                       \
        (map)->value_copy((valueptr), (_value)); \
    else                                         \
        *(valueptr) = (_value);

void 
map_init(K_TYPE, V_TYPE) (map(K_TYPE, V_TYPE) *_map) {
    _map->size = 0;
    _map->capacity = INITIAL_MAP_CAP;

    _map->buckets = (map_node(K_TYPE, V_TYPE) **) malloc(_map->capacity * sizeof(map_node(K_TYPE, V_TYPE) *));
    if (!_map->buckets) return;
    for (size_t i = 0; i < _map->capacity; ++i) _map->buckets[i] = NULL;

    _map->key_init = NULL;
    _map->key_free = NULL;
    _map->key_copy = NULL;
    _map->key_hash = NULL;
    _map->key_equals = NULL;

    _map->value_init = NULL;
    _map->value_free = NULL;
    _map->value_copy = NULL;
}

void map_node_init(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map,
        map_node(K_TYPE, V_TYPE) *node, 
        K_TYPE _key, 
        V_TYPE _value
) {
    _key_init(_map, &node->key)
    _key_copy(_map, &node->key, _key)
    _value_init(_map, &node->value)
    _value_copy(_map, &node->value, _value)
    node->next = NULL;
}

void 
map_free(K_TYPE, V_TYPE) (map(K_TYPE, V_TYPE) *_map) {
    if (_map->buckets) {
        for (size_t bucket = 0; bucket < _map->capacity; ++bucket)
            if (_map->buckets[bucket])
                map_node_free(K_TYPE, V_TYPE)(_map, _map->buckets[bucket]);
        free(_map->buckets);
    }
}

void 
map_node_free(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        map_node(K_TYPE, V_TYPE) *node
) {
    if (node->next) 
        map_node_free(K_TYPE, V_TYPE)(_map, node->next);
    _key_free(_map, &node->key)
    _value_free(_map, &node->value)
}

void
map_node_free_no_recurse(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        map_node(K_TYPE, V_TYPE) *node
) {
    _key_free(_map, &node->key)
    _value_free(_map, &node->value)
}

void 
map_node_copy(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        map_node(K_TYPE, V_TYPE) *lhs,
        map_node(K_TYPE, V_TYPE) *rhs
) {
    _key_copy(_map, &lhs->key, rhs->key)
    _value_copy(_map, &lhs->value, rhs->value)
    lhs->next = rhs->next;
}

// TODO rewrite
bool 
map_resize(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        const size_t new_cap
) {
    if (_map->buckets == NULL) { return false; }
    const size_t total_size = new_cap * sizeof(map_node(K_TYPE, V_TYPE));
    _map->buckets = (map_node(K_TYPE, V_TYPE) **) realloc(_map->buckets, total_size);
    if (_map->buckets == NULL) { return false; }
    _map->capacity = new_cap;
    return true;
}

bool 
map_insert_or_assign(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key, 
        V_TYPE _value
) {
    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;

    if (!_map->buckets[bucket]) {
        _map->buckets[bucket] = (map_node(K_TYPE, V_TYPE) *) malloc(sizeof(map_node(K_TYPE, V_TYPE)));
        map_node_init(K_TYPE, V_TYPE)(_map, _map->buckets[bucket], _key, _value);
        _map->size++;
        return true;
    }

    map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];
    do {
        if (_key_equals(_map, bucketptr->key, _key)) {
            _value_free(_map, &(bucketptr->value))
            _value_copy(_map, &(bucketptr->value), _value)
            return true;
        }
    } while ( (bucketptr = bucketptr->next) );

    bucketptr = (map_node(K_TYPE, V_TYPE) *) malloc(sizeof(map_node(K_TYPE, V_TYPE)));
    if ( !(bucketptr) ) return false;

    map_node_init(K_TYPE, V_TYPE)(_map, bucketptr, _key, _value);
    _map->size++;
    return true;
}

bool 
map_insert(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key, 
        V_TYPE _value
) {
    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;
    if (!_map->buckets[bucket]) {
        _map->buckets[bucket] = (map_node(K_TYPE, V_TYPE) *) malloc(sizeof(map_node(K_TYPE, V_TYPE)));
        map_node_init(K_TYPE, V_TYPE)(_map, _map->buckets[bucket], _key, _value);
    } else {
        map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];
        while (bucketptr->next)
            bucketptr = bucketptr->next;
        bucketptr->next = (map_node(K_TYPE, V_TYPE) *) malloc(sizeof(map_node(K_TYPE, V_TYPE)));
        if (bucketptr->next == NULL) return false;
        map_node_init(K_TYPE, V_TYPE)(_map, bucketptr->next, _key, _value);
    }
    _map->size++;
    return true;
}

bool 
map_contains(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key
) {
    if (_map->size == 0) return false;

    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;
    if (!_map->buckets[bucket]) return false;

    map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];
    do {
        if (_key_equals(_map, bucketptr->key, _key)) return true;
    } while ((bucketptr = bucketptr->next));
    return false;
}

size_t 
map_count(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key
) {
    if (_map->size == 0) return 0;

    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;
    if (!_map->buckets[bucket]) return 0;

    size_t total = 0;
    map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];
    do {
        if (_map->key_equals(_key, bucketptr->key)) ++total;
    } while ((bucketptr = bucketptr->next));
    return total;
}

bool 
map_at(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key, 
        V_TYPE *value_ret_value
) {
    if (_map->size == 0) return false;

    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;
    if (!_map->buckets[bucket]) return false;

    map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];
    do {
        if (_map->key_equals(_key, bucketptr->key)) {
            *value_ret_value = bucketptr->value;
            return true;
        }
        bucketptr = bucketptr->next;
    } while ((bucketptr = bucketptr->next));
    return false;
}

map_node(K_TYPE, V_TYPE) * 
map_find(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key
) {
    if (_map->size == 0) return NULL;

    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;
    if (!_map->buckets[bucket]) return NULL;

    map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];

    do {
        if (_key_equals(_map, bucketptr->key, _key)) return bucketptr;
    } while ( (bucketptr = bucketptr->next) );

    return NULL;
}

bool 
map_remove(K_TYPE, V_TYPE) (
        map(K_TYPE, V_TYPE) *_map, 
        K_TYPE _key
) {
    if (_map->size == 0) return false;

    const unsigned long bucket = _map->key_hash(_key) % _map->capacity;
    if (!_map->buckets[bucket]) return false;

    map_node(K_TYPE, V_TYPE) *bucketptr = _map->buckets[bucket];
    do {
        if (_key_equals(_map, bucketptr->key, _key)) {
            if ( bucketptr->next ) {
                map_node(K_TYPE, V_TYPE) *temp = bucketptr->next;
                map_node_free_no_recurse(K_TYPE, V_TYPE)(_map, bucketptr);
                bucketptr = temp;
            } else {
                map_node_free(K_TYPE, V_TYPE)(_map, bucketptr);
                bucketptr = NULL;
            }
            _map->size++;
            return true;
        }
    } while ( (bucketptr = bucketptr->next) );

    return false;
}

#undef _key_init
#undef _key_free
#undef _key_copy
#undef _key_compare
#undef _key_equals

#undef _value_init
#undef _value_free
#undef _value_copy
