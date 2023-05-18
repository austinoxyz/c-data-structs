#include "includestd.h"
#include "tokenpaste.h"

// must define K_TYPE, V_TYPE

#ifndef _HASHMAP_MACROS__
#define _HASHMAP_MACROS__

#define HASHMAP_BLOCK_SIZE (32)
#define INITIAL_HASHMAP_CAP (HASHMAP_BLOCK_SIZE)
#define NEXT_CAPACITY_UP(curr) ((HASHMAP_BLOCK_SIZE) * (((curr) / (HASHMAP_BLOCK_SIZE)) + 1))
#define MAX_HASHMAP_SIZE ((sizeof(K_TYPE) + sizeof(V_TYPE)) * (1028 * 1028))

#endif // _HASHMAP_MACROS__

#define hashmap_node(K, E) TOKENPASTE4(K, _, E, _hashmap_node)
struct hashmap_node(K_TYPE, V_TYPE) {
    K_TYPE key;
    V_TYPE value;
    struct hashmap_node(K_TYPE, V_TYPE) *next;
};
typedef struct hashmap_node(K_TYPE, V_TYPE) hashmap_node(K_TYPE, V_TYPE);

#define hashmap(K, E) TOKENPASTE4(K, _, E, _hashmap)
struct hashmap(K_TYPE, V_TYPE) {
    size_t size;
    size_t capacity;

    hashmap_node(K_TYPE, V_TYPE) **buckets;

    void (*key_init)(K_TYPE *_key);
    void (*key_free)(K_TYPE *_key);
    void (*key_copy)(K_TYPE *lhs, K_TYPE rhs);
    bool (*key_compare)(K_TYPE *lhs, K_TYPE *rhs);
    bool (*key_equals)(K_TYPE lhs, K_TYPE rhs);
    unsigned long (*key_hash)(K_TYPE key);

    void (*value_init)(V_TYPE *_value);
    void (*value_free)(V_TYPE *_value);
    void (*value_copy)(V_TYPE *lhs, V_TYPE rhs);
};
typedef struct hashmap(K_TYPE, V_TYPE) hashmap(K_TYPE, V_TYPE);

#define hashmap_node_init(K, E) TOKENPASTE4(K, _, E, _hashmap_node_init)
void hashmap_node_init(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node, 
        K_TYPE _key, 
        V_TYPE _value);

#define hashmap_node_new(K, E) TOKENPASTE4(K, _, E, _hashmap_node_new)
hashmap_node(K_TYPE, V_TYPE) *hashmap_node_new(K_TYPE, V_TYPE) ();

#define hashmap_init(K, E) TOKENPASTE4(K, _, E, _hashmap_init)
bool hashmap_init(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map);

#define hashmap_free(K, E) TOKENPASTE4(K, _, E, _hashmap_free)
void hashmap_free(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map);

#define hashmap_node_free(K, E) TOKENPASTE4(K, _, E, _hashmap_node_free)
void 
hashmap_node_free(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node);

#define hashmap_node_free_no_recurse(K, E) TOKENPASTE4(K, _, E, _hashmap_node_free_no_recurse)
void 
hashmap_node_free_no_recurse(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node);

#define hashmap_node_copy(K, E) TOKENPASTE4(K, _, E, _hashmap_node_copy)
void 
hashmap_node_copy(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *lhs,
        hashmap_node(K_TYPE, V_TYPE) *rhs);

#define hashmap_node_set_key(K, E) TOKENPASTE4(K, _, E, _hashmap_node_set_key)
void 
hashmap_node_set_key(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node,
        K_TYPE _key);

#define hashmap_node_set_value(K, E) TOKENPASTE4(K, _, E, _hashmap_node_set_value)
void 
hashmap_node_set_value(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node,
        V_TYPE _value);

#define hashmap_resize(K, E) TOKENPASTE4(K, _, E, _hashmap_resize)
bool 
hashmap_resize(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        const size_t new_cap);

#define hashmap_insert_or_assign(K, E) TOKENPASTE4(K, _, E, _hashmap_insert_or_assign)
bool 
hashmap_insert_or_assign(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key, 
        V_TYPE _value);

#define hashmap_insert(K, E) TOKENPASTE4(K, _, E, _hashmap_insert)
bool 
hashmap_insert(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key, V_TYPE _value);

#define hashmap_at(K, E) TOKENPASTE4(K, _, E, _hashmap_at)
bool 
hashmap_at(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key, V_TYPE *value_ret_value);

#define hashmap_contains(K, E) TOKENPASTE4(K, _, E, _hashmap_contains)
bool 
hashmap_contains(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key);

#define hashmap_count(K, E) TOKENPASTE4(K, _, E, _hashmap_count)
size_t 
hashmap_count(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key);

#define hashmap_find(K, E) TOKENPASTE4(K, _, E, _hashmap_find)
hashmap_node(K_TYPE, V_TYPE) * 
hashmap_find(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key);

#define hashmap_remove(K, E) TOKENPASTE4(K, _, E, _hashmap_remove)
bool 
hashmap_remove(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key);

bool 
hashmap_init(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map) {
    map->size = 0;
    map->capacity = INITIAL_HASHMAP_CAP;

    map->buckets = (hashmap_node(K_TYPE, V_TYPE) **) malloc(map->capacity * sizeof(hashmap_node(K_TYPE, V_TYPE) *));
    if (!map->buckets) return false;
    for (size_t i = 0; i < map->capacity; ++i) map->buckets[i] = NULL;

    map->key_init = NULL;
    map->key_free = NULL;
    map->key_copy = NULL;
    map->key_hash = NULL;
    map->key_equals = NULL;

    map->value_init = NULL;
    map->value_free = NULL;
    map->value_copy = NULL;

    return true;
}

void hashmap_node_init(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map,
        hashmap_node(K_TYPE, V_TYPE) *node, 
        K_TYPE _key, 
        V_TYPE _value
) {
    if (map->key_init ) 
        map->key_init(&node->key);
    if (map->value_init ) 
        map->value_init(&node->value);

    hashmap_node_set_key(K_TYPE, V_TYPE)(map, node, _key);
    hashmap_node_set_value(K_TYPE, V_TYPE)(map, node, _value);
    node->next = NULL;
}

void 
hashmap_node_set_key(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node,
        K_TYPE _key
) {
    if (map->key_copy ) 
        map->key_copy(&node->key, _key);
    else 
        node->key = _key;
}

void 
hashmap_node_set_value(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node,
        V_TYPE _value
) {
    if (map->value_copy ) 
        map->value_copy(&node->value, _value);
    else 
        node->value = _value;
}

void 
hashmap_free(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map) {
    if (map->buckets) {
        for (size_t bucket = 0; bucket < map->capacity; ++bucket)
            if (map->buckets[bucket])
                hashmap_node_free(K_TYPE, V_TYPE)(map, map->buckets[bucket]);
        free(map->buckets);
    }
}

void 
hashmap_node_free(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node
) {
    if (node->next) 
        hashmap_node_free(K_TYPE, V_TYPE)(map, node->next);
    if (map->key_free) 
        map->key_free(&node->key);
    if (map->value_free) 
        map->value_free(&node->value);
}

void
hashmap_node_free_no_recurse(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node
) {
    if (map->key_free ) 
        map->key_free(&node->key);
    if (map->value_free) 
        map->value_free(&node->value);
}

void 
hashmap_node_copy(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *lhs,
        hashmap_node(K_TYPE, V_TYPE) *rhs
) {
    if (map->key_copy)
        map->key_copy(&lhs->key, rhs->key);
    if (map->value_copy)
        map->value_copy(&lhs->value, rhs->value);
    lhs->next = rhs->next;
}

// TODO rewrite
bool 
hashmap_resize(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        const size_t new_cap
) {
    if (map->buckets == NULL) { return false; }
    const size_t total_size = new_cap * sizeof(hashmap_node(K_TYPE, V_TYPE));
    map->buckets = (hashmap_node(K_TYPE, V_TYPE) **) realloc(map->buckets, total_size);
    if (map->buckets == NULL) { return false; }
    map->capacity = new_cap;
    return true;
}

bool 
hashmap_insert_or_assign(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key, 
        V_TYPE _value
) {
    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) {
        map->buckets[bucket] = (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
        hashmap_node_init(K_TYPE, V_TYPE)(map, map->buckets[bucket], _key, _value);
    } else {
        hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];
        do {
            if (map->key_equals(_key, bucketptr->key)) {
                bucketptr->value = _value;
                return true;
            }
            bucketptr = bucketptr->next;
        } while (bucketptr );
        bucketptr->next = (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
        if (bucketptr->next == NULL) return false;
        hashmap_node_init(K_TYPE, V_TYPE)(map, bucketptr->next, _key, _value);
    }
    map->size++;
    return true;
}

bool 
hashmap_insert(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key, 
        V_TYPE _value
) {
    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) {
        map->buckets[bucket] = (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
        hashmap_node_init(K_TYPE, V_TYPE)(map, map->buckets[bucket], _key, _value);
    } else {
        hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];
        while (bucketptr->next)
            bucketptr = bucketptr->next;
        bucketptr->next = (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
        if (bucketptr->next == NULL) return false;
        hashmap_node_init(K_TYPE, V_TYPE)(map, bucketptr->next, _key, _value);
    }
    map->size++;
    return true;
}

bool 
hashmap_contains(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key
) {
    if (map->size == 0) return false;

    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) return false;

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];
    do {
        if (map->key_equals(_key, bucketptr->key)) return true;
    } while ((bucketptr = bucketptr->next) );
    return false;
}

size_t 
hashmap_count(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key
) {
    if (map->size == 0) return 0;

    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) return 0;

    size_t total = 0;
    hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];
    do {
        if (map->key_equals(_key, bucketptr->key)) ++total;
    } while ((bucketptr = bucketptr->next));
    return total;
}

bool 
hashmap_at(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key, 
        V_TYPE *value_ret_value
) {
    if (map->size == 0) return false;

    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) return false;

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];
    do {
        if (map->key_equals(_key, bucketptr->key)) {
            *value_ret_value = bucketptr->value;
            return true;
        }
        bucketptr = bucketptr->next;
    } while ((bucketptr = bucketptr->next));
    return false;
}

hashmap_node(K_TYPE, V_TYPE) * 
hashmap_find(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key
) {
    if (map->size == 0) return false;

    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) return false;

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];

    if (map->key_equals(_key, bucketptr->key)) {
        return bucketptr;
    } else {
        while (bucketptr->next) {
            bucketptr = bucketptr->next;
            if (map->key_equals(_key, bucketptr->key)) {
                return bucketptr;
            }
        }
    }
    return false;
}

bool 
hashmap_remove(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        K_TYPE _key
) {
    if (map->size == 0) return false;

    const unsigned long bucket = map->key_hash(_key) % map->capacity;
    if (!map->buckets[bucket]) return false;

    if (map->key_equals(_key, map->buckets[bucket]->key)) {
        if (map->buckets[bucket]->next) {
            hashmap_node(K_TYPE, V_TYPE) *temp = map->buckets[bucket]->next;
            hashmap_node_free_no_recurse(K_TYPE, V_TYPE)(map, map->buckets[bucket]);
            map->buckets[bucket] = temp;
        } else {
            hashmap_node_free(K_TYPE, V_TYPE)(map, map->buckets[bucket]);
        }
        map->size--;
        return true;
    }

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = map->buckets[bucket];
    while (bucketptr->next) {
        if (map->key_equals(_key, bucketptr->next->key)) {
            hashmap_node(K_TYPE, V_TYPE) *temp = bucketptr->next->next;
            hashmap_node_free(K_TYPE, V_TYPE)(map, bucketptr->next);
            bucketptr->next = temp;
            return true;
        }
        bucketptr = bucketptr->next;
    }
    return false;
}

