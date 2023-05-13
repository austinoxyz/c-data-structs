#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h> 
#include <string.h>

// must define K_TYPE, V_TYPE

#ifndef _HASHMAP_MACROS__
#define _HASHMAP_MACROS__

#define HASHMAP_BLOCK_SIZE (32)
#define INITIAL_HASHMAP_CAP (HASHMAP_BLOCK_SIZE)
#define NEXT_CAPACITY_UP(curr) ((HASHMAP_BLOCK_SIZE) * (((curr) / (HASHMAP_BLOCK_SIZE)) + 1))
#define MAX_HASHMAP_SIZE ((sizeof(K_TYPE) + sizeof(V_TYPE)) * (1028 * 1028))

#define TOKENPASTE2(x, y) x ## y
#define TOKENPASTE3(x, y, z) x ## y ## z
#define TOKENPASTE4(a, b, c, d) a ## b ## c ## d
#define TOKENPASTE5(a, b, c, d, e) a ## b ## c ## d ## e

#endif // _VEC_MACROS__

#define hashmap_node(K, E) TOKENPASTE4(K, _, E, _hashmap_node)
struct hashmap_node(K_TYPE, V_TYPE) {
    K_TYPE key;
    V_TYPE value;
    struct hashmap_node(K_TYPE, V_TYPE) *next;
};

typedef struct hashmap_node(K_TYPE, V_TYPE) hashmap_node(K_TYPE, V_TYPE);

#define hashmap_node_init(K, E) TOKENPASTE4(K, _, E, _hashmap_node_init)
void hashmap_node_init(K_TYPE, V_TYPE) (hashmap_node(K_TYPE, V_TYPE) *node, K_TYPE _key, V_TYPE _value);

#define hashmap_node_new(K, E) TOKENPASTE4(K, _, E, _hashmap_node_new)
hashmap_node(K_TYPE, V_TYPE) *hashmap_node_new(K_TYPE, V_TYPE) ();

void hashmap_node_init(K_TYPE, V_TYPE) (hashmap_node(K_TYPE, V_TYPE) *node, K_TYPE _key, V_TYPE _value) {
    node->key = _key;
    node->value = _value;
    node->next = NULL;
}

hashmap_node(K_TYPE, V_TYPE) *hashmap_node_new(K_TYPE, V_TYPE) () {
    return (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
}

#define hashmap(K, E) TOKENPASTE4(K, _, E, _hashmap)
struct hashmap(K_TYPE, V_TYPE) {
    size_t size;
    size_t capacity;

    hashmap_node(K_TYPE, V_TYPE) *buckets;
    bool *allocated_buckets;

    void (*ptr_free_k)(K_TYPE *_key);
    void (*ptr_free_v)(V_TYPE *_value);
    void (*ptr_copy_k)(K_TYPE *lhs, K_TYPE *rhs);
    void (*ptr_copy_v)(V_TYPE *lhs, V_TYPE *rhs);
    unsigned long (*ptr_hash)(K_TYPE key);
    bool (*ptr_key_equals)(K_TYPE lhs, K_TYPE rhs);

    K_TYPE null_elem;
};

typedef struct hashmap(K_TYPE, V_TYPE) hashmap(K_TYPE, V_TYPE);

#define hashmap_init(K, E) TOKENPASTE4(K, _, E, _hashmap_init)
bool hashmap_init(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        unsigned long (*hash)(K_TYPE key), 
        bool (*key_equals)(K_TYPE lhs, K_TYPE rhs));

#define hashmap_free(K, E) TOKENPASTE4(K, _, E, _hashmap_free)
void hashmap_free(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map);

#define hashmap_hashmap_node_free(K, E) TOKENPASTE4(K, _, E, _hashmap_hashmap_node_free)
void hashmap_hashmap_node_free(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node);

#define hashmap_hashmap_node_copy(K, E) TOKENPASTE4(K, _, E, _hashmap_hashmap_node_copy)
void hashmap_hashmap_node_copy(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *lhs,
        hashmap_node(K_TYPE, V_TYPE) *rhs);

#define hashmap_set_free_key_func(K, E) TOKENPASTE4(K, _, E, _hashmap_set_free_key_func)
void hashmap_set_free_key_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*free_key)(K_TYPE *_key));

#define hashmap_set_free_value_func(K, E) TOKENPASTE4(K, _, E, _hashmap_set_free_value_func)
void hashmap_set_free_value_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*free_value)(V_TYPE *_value));

#define hashmap_set_copy_key_func(K, E) TOKENPASTE4(K, _, E, _hashmap_set_copy_key_func)
void hashmap_set_copy_key_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*copy_key)(K_TYPE *lhs, K_TYPE *rhs));

#define hashmap_set_copy_value_func(K, E) TOKENPASTE4(K, _, E, _hashmap_set_copy_value_func)
void hashmap_set_copy_value_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*copy_value)(V_TYPE *lhs, V_TYPE *rhs));

#define hashmap_resize(K, E) TOKENPASTE4(K, _, E, _hashmap_resize)
bool hashmap_resize(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, const size_t new_cap);

#define hashmap_insert_or_assign(K, E) TOKENPASTE4(K, _, E, _hashmap_insert_or_assign)
bool hashmap_insert_or_assign(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key, V_TYPE _value);

#define hashmap_insert(K, E) TOKENPASTE4(K, _, E, _hashmap_insert)
bool hashmap_insert(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key, V_TYPE _value);

#define hashmap_at(K, E) TOKENPASTE4(K, _, E, _hashmap_at)
bool hashmap_at(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key, V_TYPE *value_ret_value);

#define hashmap_contains(K, E) TOKENPASTE4(K, _, E, _hashmap_contains)
bool hashmap_contains(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key);

#define hashmap_count(K, E) TOKENPASTE4(K, _, E, _hashmap_count)
size_t hashmap_count(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key);

#define hashmap_remove(K, E) TOKENPASTE4(K, _, E, _hashmap_remove)
bool hashmap_remove(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key);

bool hashmap_init(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        unsigned long (*hash)(K_TYPE key),
        bool (*key_equals)(K_TYPE lhs, K_TYPE rhs)
) {
    map->size = 0;
    map->capacity = INITIAL_HASHMAP_CAP;

    map->allocated_buckets = (bool *) malloc(map->capacity * sizeof(bool));
    if (map->allocated_buckets == NULL) { return false; }
    memset(map->allocated_buckets, false, map->capacity * sizeof(bool));
    if (map->allocated_buckets == NULL) { return false; }

    map->buckets = (hashmap_node(K_TYPE, V_TYPE) *) malloc(map->capacity * sizeof(hashmap_node(K_TYPE, V_TYPE)));
    if (map->buckets == NULL) { return false; }

    map->ptr_hash = hash;
    map->ptr_key_equals = key_equals;
    map->ptr_free_k = NULL;
    map->ptr_free_v = NULL;
    map->ptr_copy_k = NULL;
    map->ptr_copy_v = NULL;
    return true;
}

void hashmap_set_free_key_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*free_key)(K_TYPE *_key)) {
    map->ptr_free_k = free_key;
}

void hashmap_set_free_value_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*free_value)(V_TYPE *_value)) {
    map->ptr_free_v = free_value;
}

void hashmap_set_copy_key_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*copy_key)(K_TYPE *lhs, K_TYPE *rhs)) {
    map->ptr_copy_k = copy_key;
}

void hashmap_set_copy_value_func(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, void (*copy_value)(V_TYPE *lhs, V_TYPE *rhs)) {
    map->ptr_copy_v = copy_value;
}

void hashmap_free(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map) {
    if (map->buckets != NULL) {
        for (size_t i = 0; i < map->capacity; ++i)
            if (map->allocated_buckets[i])
                hashmap_hashmap_node_free(K_TYPE, V_TYPE)(map, &map->buckets[i]);
        free(map->buckets);
    }
    if (map->allocated_buckets != NULL) { 
        free(map->allocated_buckets); 
    }
}

void hashmap_hashmap_node_free(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *node
) {
    if (map->ptr_free_k != NULL)
        map->ptr_free_k(&node->key);
    if (map->ptr_free_v != NULL)
        map->ptr_free_v(&node->value);
}

void hashmap_hashmap_node_copy(K_TYPE, V_TYPE) (
        hashmap(K_TYPE, V_TYPE) *map, 
        hashmap_node(K_TYPE, V_TYPE) *lhs,
        hashmap_node(K_TYPE, V_TYPE) *rhs
) {
    if (map->ptr_copy_k != NULL)
        map->ptr_copy_k(&lhs->key, &rhs->key);
    if (map->ptr_copy_v != NULL)
        map->ptr_copy_v(&lhs->value, &rhs->value);
}

bool hashmap_resize(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, const size_t new_cap) {
    if (map->buckets == NULL) { return false; }
    const size_t total_size = new_cap * sizeof(hashmap_node(K_TYPE, V_TYPE));
    map->buckets = (hashmap_node(K_TYPE, V_TYPE) *) realloc(map->buckets, total_size);
    if (map->buckets == NULL) { return false; }
    map->capacity = new_cap;
    return true;
}

bool hashmap_insert_or_assign(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key, V_TYPE _value) {
    const unsigned long bucket = map->ptr_hash(_key) % map->capacity;
    if (!map->allocated_buckets[bucket]) {
        hashmap_node_init(K_TYPE, V_TYPE)(&map->buckets[bucket], _key, _value);
        map->allocated_buckets[bucket] = true;
    } else {
        hashmap_node(K_TYPE, V_TYPE) *bucketptr = &(map->buckets[bucket]);
        do {
            if (map->ptr_key_equals(_key, bucketptr->key)) {
                printf("keys are equal!\n");
                bucketptr->value = _value;
                return true;
            }
            bucketptr = bucketptr->next;
        } while (bucketptr->next != NULL);
        bucketptr->next = (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
        if (bucketptr->next == NULL) { return false; }
        hashmap_node_init(K_TYPE, V_TYPE)(bucketptr->next, _key, _value);
    }
    map->size++;
    return true;
}

bool hashmap_insert(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key, V_TYPE _value) {
    const unsigned long bucket = map->ptr_hash(_key) % map->capacity;
    if (!map->allocated_buckets[bucket]) {
        hashmap_node_init(K_TYPE, V_TYPE)(&map->buckets[bucket], _key, _value);
        map->allocated_buckets[bucket] = true;
    } else {
        hashmap_node(K_TYPE, V_TYPE) *bucketptr = &(map->buckets[bucket]);
        while (bucketptr->next != NULL) {
            bucketptr = bucketptr->next;
        }
        bucketptr->next = (hashmap_node(K_TYPE, V_TYPE) *) malloc(sizeof(hashmap_node(K_TYPE, V_TYPE)));
        if (bucketptr->next == NULL) { return false; }
        hashmap_node_init(K_TYPE, V_TYPE)(bucketptr->next, _key, _value);
    }
    map->size++;
    return true;
}

bool hashmap_contains(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key) {
    if (map->size == 0) { return false; }

    const unsigned long bucket = map->ptr_hash(_key) % map->capacity;
    if (!map->allocated_buckets[bucket]) { return false; }

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = &(map->buckets[bucket]);
    do {
        if (map->ptr_key_equals(_key, bucketptr->key)) 
            return true;
    } while ((bucketptr = bucketptr->next) != NULL);
    return false;
}

size_t hashmap_count(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key) {
    if (map->size == 0) { return 0; }

    const unsigned long bucket = map->ptr_hash(_key) % map->capacity;
    if (!map->allocated_buckets[bucket]) { return 0; }

    size_t total = 0;
    hashmap_node(K_TYPE, V_TYPE) *bucketptr = &(map->buckets[bucket]);
    do {
        if (map->ptr_key_equals(_key, bucketptr->key)) 
            ++total;
    } while ((bucketptr = bucketptr->next) != NULL);
    return total;
}

bool hashmap_at(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key, V_TYPE *value_ret_value) {
    if (map->size == 0) { return false; }

    const unsigned long bucket = map->ptr_hash(_key) % map->capacity;
    if (!map->allocated_buckets[bucket]) { return false; }

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = &(map->buckets[bucket]);
    do {
        if (map->ptr_key_equals(_key, bucketptr->key)) {
            *value_ret_value = bucketptr->value;
            return true;
        }
        bucketptr = bucketptr->next;
    } while ((bucketptr = bucketptr->next) != NULL);
    return false;
}

bool hashmap_remove(K_TYPE, V_TYPE) (hashmap(K_TYPE, V_TYPE) *map, K_TYPE _key) {
    if (map->size == 0) { return false; }

    const unsigned long bucket = map->ptr_hash(_key) % map->capacity;
    if (!map->allocated_buckets[bucket]) { return false; }

    hashmap_node(K_TYPE, V_TYPE) *bucketptr = &(map->buckets[bucket]);

    if (map->ptr_key_equals(_key, bucketptr->key)) {
        printf("removing first element.\n");
//        memcpy(&map->buckets[bucket], map->buckets[bucket].next, sizeof(hashmap_node(K_TYPE, V_TYPE)));
        hashmap_hashmap_node_copy(string, int)(map, bucketptr, bucketptr->next);
        bucketptr->next = NULL;
        map->size--;
        return true;
    }

    while (bucketptr->next != NULL) {
        if (map->ptr_key_equals(_key, bucketptr->next->key)) {
            hashmap_node(K_TYPE, V_TYPE) *temp = bucketptr->next->next;
            hashmap_hashmap_node_free(K_TYPE, V_TYPE)(map, bucketptr->next);
            bucketptr->next = temp;
            return true;
        }
        bucketptr = bucketptr->next;
    }
    return false;
}

