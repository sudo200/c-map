#include <map.h>

struct pair
{
  void * key;
  void * value;
};

struct map
{
  pair *pairs;

  size_t key_size;

  alloc_t alloc;
  dealloc_t dealloc;
};

void * __resize(alloc_t alloc, dealloc_t dealloc, void * ptr, size_t bytes_to_copy, size_t bytes_to_alloc)
{
  void * newptr = memmove(
        alloc(bytes_to_alloc),
        ptr,
        bytes_to_copy
      );
  dealloc(ptr);
  return newptr;
}

#ifndef NO_STDLIB
map * map_new(size_t key_size)
{
  return map_new_custom_alloc(malloc, free, key_size);
}
#endif//NO_STDLIB

map * map_new_custom_alloc(alloc_t alloc, dealloc_t dealloc, size_t key_size)
{
  map * m = (map *) alloc(sizeof(map));

  pair * p = (pair *) alloc(sizeof(pair));

  *p = (pair) {
      NULL,
      NULL,
  };

  *m = (map) {
      p,

      key_size,

      alloc,
      dealloc,
  };

  return m;
}

size_t map_size(map * m)
{
  if(m == NULL)
    return 0;

  size_t len = 0;
  for(pair *p = m->pairs; * (int *) p; p++, len++);
  return len;
}

map * map_add(map * m, void * key, void * value)
{
  if(m == NULL)
    return NULL;

  const size_t size = map_size(m);

  m->pairs = __resize(
        m->alloc,
        m->dealloc,
        m->pairs,
        (size + 1) * sizeof(pair),
        (size + 2) * sizeof(pair)
      );

  m->pairs[size] = (pair) {
    key,
    value,
  };

  m->pairs[size + 1] = (pair) {
    NULL,
    NULL,
  };

  return m;
}

void * map_get(map * m, void * key)
{
  if(m == NULL)
    return NULL;

  for(pair *p = m->pairs; * (int *) p; p++)
    if(
        m->key_size == 0
        ? strcmp((const char *) p->key, (const char *) key) == 0
        : memcmp(p->key, key, m->key_size) == 0
      )
      return p->value;
  
  return NULL;
}

void * map_remove(map * m, void * key)
{
  if(m == NULL)
    return NULL;

  const size_t size = map_size(m);

  pair *p = m->pairs;
  for(; * (int *) p; p++)
    if(
        m->key_size == 0
        ? strcmp((const char *) p->key, (const char *) key) == 0
        : memcmp(p->key, key, m->key_size) == 0
      )
      goto here;
  return NULL;

  here:;

  const size_t index = p - m->pairs;
  void * value = p->value;

  for(size_t i = index; i < size; i++)
    m->pairs[i] = m->pairs[i + 1];

  m->pairs = __resize(
        m->alloc,
        m->dealloc,
        m->pairs,
        (size - 1) * sizeof(pair),
        (size - 1) * sizeof(pair)
      );

  return value;
}

int map_contains_key(map * m, void * key)
{
  if(m == NULL)
    return 0;

  for(pair *p = m->pairs; * (int *) p; p++)
    if(
        m->key_size == 0
        ? strcmp((const char *) p->key, (const char *) key) == 0
        : memcmp(p->key, key, m->key_size) == 0
      )
      return 1;

  return 0;
}

 map * map_foreach(map * m, biconsumer_t consumer)
{
  if(m == NULL)
    return NULL;

  if(consumer == NULL)
    return m;

  for(pair * p = m->pairs; * (int *) p; p++)
    consumer(p->key, p->value);
  
  return m;
}

void map_destroy(map * m)
{
  if(m == NULL)
    return;

  if(* (int *) m->pairs)
    m->dealloc(m->pairs);

  m->dealloc(m);
}

