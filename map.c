#include "map.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


MapEntry *map_entry_init(int key, void *value) {
  MapEntry *entry = malloc(sizeof(MapEntry));
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  return entry;
}

void map_entry_free(MapEntry *entry) {
  while (entry && entry->next) {
    map_entry_free(entry->next);
  }
  free(entry);
}


HashMap *hash_map_init(size_t map_size) {
  HashMap *map = malloc(sizeof(map_size));
  map->count = 0;
  map->size = map_size;
  map->entries = malloc(sizeof(MapEntry) * map_size);
  size_t i;
  for (i = 0; i < map_size; ++i) {
    map->entries[i] = NULL;
  }
  return map;
}

void hash_map_free(HashMap *map) {
  size_t i;
  for (i = 0; i < map->size; ++i) {
    map_entry_free(map->entries[i]);
    map->entries[i] = NULL;
  }
  free(map);
}

size_t hash_function(HashMap *map, int key) {
  return key % map->size;
}

void hash_map_insert(HashMap *map, int key, void *value){
  MapEntry *entry = map_entry_init(key, value);
  size_t slot = hash_function(map, key);
  if (map->entries[slot] == NULL) {
    map->entries[slot] = entry;
  } else {
    MapEntry *iter = map->entries[slot];
    while (iter->next) {
      iter = iter->next;
    }
    iter->next = entry;
  }
  map->count += 1;
}

bool hash_map_get(HashMap *map, int key, void *value) {
  size_t slot = hash_function(map, key);
  MapEntry *iter = map->entries[slot];
  while (iter) {
    if (iter->key == key) {
      value = iter->value;
      return true;
    }
    iter = iter->next;
  }
  return false;
}

void *hash_map_delete(HashMap *map, int key) {
  size_t slot = hash_function(map, key);
  MapEntry *iter = map->entries[slot];
  MapEntry *prv = NULL;
  while (iter) {
    if (iter->key == key) {
      if (!prv) {
        map->entries[slot] = iter->next;
      } else {
        prv->next = iter->next;
      }
      iter->next = NULL;
      void *value = iter->value;
      map_entry_free(iter);
      map->count -= 1;
      return value;
    }
    prv = iter;
    iter = iter->next;
  }
  return NULL;
}

void print_map(HashMap *map, const char *title) {
  printf("%s: (%ld/%ld):\n", title, map->count, map->size);
  size_t i;
  for (i = 0; i < map->size; ++i) {
    MapEntry *iter = map->entries[i];
    if (iter) {
      printf("\t[%ld]: ", i);
      while (iter) {
        printf("(%d)", iter->key);
        iter = iter->next;
      }
      printf("\n");
    }
  }
}

int main() {
  HashMap *map = hash_map_init(100);
  print_map(map, "initial status");

  hash_map_insert(map, 12, NULL);
  hash_map_insert(map, 5, NULL);
  hash_map_insert(map, 305, NULL);
  print_map(map, "after insert");

  hash_map_delete(map, 5);
  print_map(map, "after remove");

  hash_map_free(map);
  return EXIT_SUCCESS;
}
