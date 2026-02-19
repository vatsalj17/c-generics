#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hashtable.h"

typedef struct {
    char* userid;
    char* name;
    int age;
} Person;

void cleanup(void* obj) {
    if (obj == NULL) return;
    Person* p = (Person*)obj;
    free(p->userid);
    free(p->name);
    free(p);
}

uint64_t hash_function(const char* str, size_t len) {
    uint32_t hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str[i];  // hash * 33 + c
    }
    return hash;
}

int main(void) {
    hash_table* ht = hash_table_create(10, hash_function, cleanup);
    if (ht == NULL) {
        fprintf(stderr, "Failed to create hash table\n");
        return 1;
    }
    Person* p1 = malloc(sizeof(Person));
    p1->userid = strdup("user123");
    p1->name = strdup("Alice Johnson");
    p1->age = 30;
    Person* p2 = malloc(sizeof(Person));
    p2->userid = strdup("user456");
    p2->name = strdup("Bob Smith");
    p2->age = 25;
    Person* p3 = malloc(sizeof(Person));
    p3->userid = strdup("user789");
    p3->name = strdup("Charlie Brown");
    p3->age = 35;

    if (!hash_table_insert(ht, p1->userid, p1)) {
        fprintf(stderr, "Failed to insert user123\n");
    }
    if (!hash_table_insert(ht, p2->userid, p2)) {
        fprintf(stderr, "Failed to insert user456\n");
    }
    if (!hash_table_insert(ht, p3->userid, p3)) {
        fprintf(stderr, "Failed to insert user789\n");
    }
    hash_table_print(ht);

    Person* found = (Person*)hash_table_lookup(ht, "user456");
    if (found) {
        printf("Found: %s (age %d)\n", found->name, found->age);
    } else {
        printf("User456 not found\n");
    }

    void* deleted = hash_table_delete(ht, "user123");
    if (deleted) {
        printf("Deleted user123\n");
    }

    hash_table_print(ht);
    hash_table_destroy(ht);
}
