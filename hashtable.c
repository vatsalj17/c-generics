#include "hashtable.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct entry {
	char* key;
	void* obj;
    struct entry* next;
} entry;

typedef struct _hash_table {
	uint32_t size;
	hashfunction* hash;
    cleanupfunction* cf;
	entry** elements;
} hash_table;

static size_t hash_table_index(hash_table *ht, const char *key) {
    size_t result = ht->hash(key, strlen(key)) % ht->size;
    return result;
}

hash_table* hash_table_create(uint32_t size, hashfunction* hf, cleanupfunction* cf) {
    hash_table* ht = malloc(sizeof(hash_table));
    ht->size = size;
    ht->hash = hf;
    if (cf) {
        ht->cf = cf;
    } else {
        ht->cf = free;
    }
    ht->elements = calloc(ht->size, sizeof(entry*));
    return ht;
}

void hash_table_destroy(hash_table* ht) {
    if (ht == NULL) return;
    for (uint32_t i = 0; i < ht->size; i++) {
        while (ht->elements) {
            entry* temp = ht->elements[i];
            if (temp == NULL) break;
            ht->elements[i] = ht->elements[i]->next;
            free(temp->key);
            ht->cf(temp->obj);
            free(temp);
        }
    }
    free(ht->elements);
    free(ht);
}

void hash_table_print(hash_table* ht) {
    printf("\t-- HASH TABLE --\n");
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->elements[i] == NULL) continue;
        printf("\t%d\t", i);
        entry *temp = ht->elements[i];
        while (temp != NULL) {
            printf("\"%s\"(%p) -> ", temp->key, temp->obj);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\t--- THE END ---\n");
}

bool hash_table_insert(hash_table* ht, const char* key, void* obj) {
    if (ht == NULL || key == NULL || obj == NULL) return false;
    if (hash_table_lookup(ht, key) != NULL) return false;
    size_t index = hash_table_index(ht, key);

    // creating entry
    entry *e = malloc(sizeof(entry));
    e->obj = obj;
    // e->key = malloc(strlen(key) + 1);
    // strcpy(e->key, key);
    e->key = strdup(key);
    
    // inserting entry
    e->next = ht->elements[index];
    ht->elements[index] = e;
    return true;
}

void* hash_table_lookup(hash_table* ht, const char* key) {
    if (key == NULL || ht == NULL) return false;
    size_t index = hash_table_index(ht, key);
    entry *temp = ht->elements[index];
    while (temp != NULL && strcmp(temp->key, key) != 0) {
        temp = temp->next;
    }
    if (temp == NULL) return NULL;
    return temp->obj;
}

void* hash_table_delete(hash_table* ht, const char* key) {
    if (key == NULL || ht == NULL) return false;
    size_t index = hash_table_index(ht, key);
    entry *temp = ht->elements[index];
    entry *prev = NULL;
    while (temp != NULL && strcmp(temp->key, key) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return NULL;
    if (prev == NULL) {
        // deleting the head node
        ht->elements[index] = temp->next;
    } else {
        prev->next = temp->next;
    }
    void *result = temp->obj;
    free(temp);
    return result;
}
