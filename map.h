/**
 * Simple HashMap implementation in C
 * with int as keys and void* as values
 * Time-stamp: <2023-02-22 11:44:14 by Reza Majd (reza-lenovo)>
 */

#ifndef MAP_H_
#define MAP_H_

#include <stdlib.h>
#include <stdbool.h>


typedef struct _MapEntry {
  int key;
  void *value;
  struct _MapEntry *next;
} MapEntry;

MapEntry *map_entry_init(int key, void *value);
void map_entry_free(MapEntry *entry);


typedef struct {
  MapEntry **entries;
  size_t count;
  size_t size;
} HashMap;

HashMap *hash_map_init(size_t map_size);
void hash_map_free(HashMap *map);
void hash_map_insert(HashMap *map, int key, void *value);
bool hash_map_get(HashMap *map, int key, void *value);
void *hash_map_delete(HashMap *map, int key);

#endif
