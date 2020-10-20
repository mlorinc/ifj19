#ifndef __H_SYMTABLE
#define __H_SYMTABLE

#include <stdbool.h>
#include "queue.h"

struct item
{
	char *key;
	void *data;
	struct item* ptrnext;
};

typedef struct item* item_t;
typedef struct hash_map *hash_map_t;
typedef unsigned (*hash_function_t)(char *key);

struct hash_map_iterator
{
	size_t bucket_index;
	item_t current;
	hash_map_t map;
};

typedef struct hash_map_iterator hash_map_iterator_t;

/**
 * Initialization new hash map.
 * @return pointer on new hash map or NULL on malloc error.
 */
hash_map_t hash_map_init();

/**
 * Insert item.
 * @param hash_map place where I can insert.
 * @param key searching key.
 * @param element inserting item.
 * @return if operation was successful true, else false.
 */
bool hash_map_put(hash_map_t hash_map, char *key, void *element);

/**
 * Searching item by key.
 * @param hash_map place where I can searching.
 * @param key searching key.
 * @return if item was found true, else false.
 */
bool hash_map_has(hash_map_t hash_map, char *key);

/**
 * Searching item by key.
 * @param hash_map place where I can searching.
 * @param key searching key.
 * @return if item was found it will be return, else NULL.
 */
void *hash_map_get(hash_map_t hash_map, char *key);

/**
 * Find out if hash map is empty.
 * @param hash_map
 * @return if it's empty true, else false.
 */
bool hash_map_clear(hash_map_t hash_map);

/**
 * Destroy entire hash map.
 * @param hash_map for destroy.
 */
void hash_map_destroy(hash_map_t hash_map);

/**
 * Returns size of hash map
 * @param size of hash map
 */
size_t hash_map_size(hash_map_t hash_map);

/**
 * Delete item in hash map.
 * @param hash_map where I can delete item.
 * @param key item key.
 */
void hash_map_delete_item (hash_map_t hash_map, char* key);

void hash_map_set_hash_function(hash_map_t hash_map, hash_function_t hash_function);

hash_map_iterator_t hash_map_begin(hash_map_t map);

hash_map_iterator_t hash_map_iterator_next(hash_map_iterator_t ite);

void *hash_map_iterator_value(hash_map_iterator_t ite);

#endif
