#ifndef __H_HASH_MAP
#define __H_HASH_MAP

#include <stdbool.h>

struct hash_map;
typedef struct hash_map* hash_map_t;

#define BUCKET_SIZE 60

hash_map_t hash_map_init();
bool hash_map_put(hash_map_t hash_map, const char *key, void *element);
bool hash_map_has(hash_map_t hash_map, const char *key);
void *hash_map_get(hash_map_t hash_map, const char *key);
bool hash_map_clear(hash_map_t hash_map);
bool hash_map_destroy(hash_map_t hash_map);

void hash_map_set_hash_function(hash_map_t hash_map, );

#undef BUCKET_SIZE
#endif