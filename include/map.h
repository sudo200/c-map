#ifndef  MAP_H
#define  MAP_H

#ifndef NO_STDLIB
#include <stdlib.h>// For malloc and free
#endif//NO_STDLIB

#include <string.h>// For memcpy, etc.

typedef struct map map;
typedef struct pair pair;

typedef void * (*alloc_t)(size_t);
typedef void (*dealloc_t)(void *);

typedef void (*biconsumer_t)(void *, void *);

/**
 * Allocates a new map.
 * Like `map_new()`, but allows
 * you to specify your own allocator and
 * deallocator function.
 *
 * If using strings or char-sized,
 * NUL-terminated arrays as a key,
 * the size can be set to 0.
 */
map * map_new_custom_alloc(alloc_t alloc, dealloc_t dealloc, size_t key_size);

#ifndef NO_STDLIB
/**
 * Allocates a new map.
 * Uses `malloc` and `free` for memory managment.
 * If using strings or char-sized,
 * NUL-terminated arrays as a key,
 * the size can be set to 0.
 */
map * map_new(size_t key_size);
#endif//NO_STDLIB

/**
 * Returns the amount of elements
 * stored in the map.
 */
size_t map_size(map * m);

/**
 * Adds a new element
 * to the map.
 */
map * map_add(map * m, void * key, void * value);

/**
 * Gets the value stored behind key.
 * Returns NULL, if such a key does not exist.
 */
void * map_get(map * m, void * key);

/**
 * Return true, if the given key exists.
 * Returns false otherwise.
 */
int map_contains_key(map * m, void * key);

/**
 * Calls given function for each key-value pair
 * in the map.
 */
map * map_foreach(map * m, biconsumer_t consumer);

/**
 * Destroys the map.
 *
 * Elements have to be deallocated separately.
 */
void map_destroy(map * m);

#endif// MAP_H

