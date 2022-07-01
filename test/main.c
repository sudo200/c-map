#include <map.h>

#include <stdio.h>

void myconsumer(void * key, void * value)
{
  printf("Key: %s\tValue: %s\n", (const char *) key, (const char *) value);
}

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    puts("Please provide a key as the first program argument!");
    return -1;
  }

  map * mymap = map_new(0);
  puts("Created map");

  printf("Size out of the box: %lu\n", map_size(mymap));

  map_add(mymap, "Key", "Value");
  map_add(mymap, "Foo", "Bar");
  map_add(mymap, "lol", NULL);

  printf("Size after adding elements: %lu\n", map_size(mymap));

  printf("Removed element: %s\n", (const char *) map_remove(mymap, argv[1]));

  printf("Element with key \"%s\" exists: %s\n", argv[1], map_contains_key(mymap, argv[1]) ? "Yes" : "No");
  printf("Element with key \"%s\": %s\n", argv[1], (const char *) map_get(mymap, argv[1]));

  puts("All Elements:\n+-------------+");
  map_foreach(mymap, myconsumer);

  map_destroy(mymap);
  puts("Destroyed map");

  return 0;
}

