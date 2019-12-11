#include "symtable.h"
#include <string.h>

#define BUCKET_SIZE 60
struct hash_map
{
	item_t *buckets;
	size_t size;
};

/**
 * Generate index for hash map.
 * @param key item key.
 */
int hash_code(char *key)
{
	int retval = 1;
	int keylen = strlen(key);
	for (int i = 0; i < keylen; i++)
		retval += key[i];
	return (retval % BUCKET_SIZE);
}

/**
 * Searching for hash map item.
 * @param hash_map place where I can searching.
 * @param key searching key.
 * @return if hash map item was found it will be return, else NULL.
 */
void *hash_search(hash_map_t hash_map, char *key)
{
	item_t actual = NULL;
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		actual = hash_map->buckets[i]; //TODO 100% wokring :kappa:
		while (actual != NULL)
		{
			if (strcmp(actual->key, key) == 0)
			{
				return actual;
			}
			actual = actual->ptrnext;
		}
	}
	return NULL;
}

hash_map_t hash_map_init()
{
	hash_map_t map = malloc(sizeof(struct hash_map));
	map->size = 0;
	map->buckets = malloc(sizeof(item_t) * BUCKET_SIZE);

	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		map->buckets[i] = NULL;
	}
	return map;
}

bool hash_map_put(hash_map_t hash_map, char *key, void *element)
{
	item_t foundItem = hash_search(hash_map, key);
	if (foundItem != NULL)
	{
		foundItem->data = element;
		return true;
	}
	else
	{
		int id = hash_code(key);
		item_t new = (item_t)malloc(sizeof(struct item));
		if (new == NULL)
		{
			return false;
		}
		new->data = element;

		size_t length = strlen(key) + 1;
		new->key = malloc(length);
		memcpy(new->key, key, length);

		new->ptrnext = hash_map->buckets[id];
		hash_map->buckets[id] = new;
		hash_map->size++;
		return true;
	}
}

bool hash_map_has(hash_map_t hash_map, char *key)
{
	if (hash_search(hash_map, key) != NULL)
	{
		return true;
	}
	return false;
}

void *hash_map_get(hash_map_t hash_map, char *key)
{
	item_t found_item = hash_search(hash_map, key);
	if (found_item != NULL)
	{
		return found_item->data;
	}
	return NULL;
}

bool hash_map_clear(hash_map_t hash_map)
{
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		if (hash_map->buckets[i] != NULL)
		{
			return false;
		}
	}
	return true;
}

void hash_map_destroy(hash_map_t hash_map)
{
	item_t actual = NULL;
	item_t next = NULL;
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		actual = hash_map->buckets[i];
		while (actual != NULL)
		{
			next = actual->ptrnext;
			// free(actual->data);
			free(actual->key);
			free(actual);
			actual = next;
		}
		hash_map->buckets[i] = NULL;
	}
	free(hash_map);
}

void hash_map_delete_item(hash_map_t hash_map, char *key)
{
	item_t actual = NULL;
	item_t previous = NULL;
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		actual = hash_map->buckets[i];
		if (actual != NULL)
		{
			if (strcmp(actual->key, key) == 0)
			{
				hash_map->buckets[i] = actual->ptrnext;
				free(actual);
				hash_map->size--;
				return;
			}
			previous = actual;
			actual = previous->ptrnext;
			while (actual != NULL)
			{
				if (strcmp(actual->key, key) == 0)
				{
					previous->ptrnext = actual->ptrnext;
					free(actual);
					hash_map->size--;
					return;
				}
				previous = actual;
				actual = previous->ptrnext;
			}
		}
	}
}

hash_map_iterator_t hash_map_begin(hash_map_t map)
{
	hash_map_iterator_t ite;
	ite.map = map;

	for (size_t i = 0; i < BUCKET_SIZE; i++)
	{
		item_t bucket = map->buckets[i];

		if (bucket != NULL) {
			ite.bucket_index = i;
			ite.current = bucket;
			return ite;
		}
	}
	
	ite.bucket_index = 0;
	ite.current = NULL;
	return ite;
}

hash_map_iterator_t hash_map_iterator_next(hash_map_iterator_t ite)
{
	if (ite.current->ptrnext) {
		ite.current = ite.current->ptrnext;
		return ite;
	}
	
	for (size_t i = ite.bucket_index + 1; i < BUCKET_SIZE; i++)
	{
		item_t bucket = ite.map->buckets[i];

		if (bucket != NULL) {
			ite.bucket_index = i;
			ite.current = bucket;
			return ite;
		}
	}

	ite.current = NULL;
	return ite;
}

void *hash_map_iterator_value(hash_map_iterator_t ite) {
	return ite.current;
}
