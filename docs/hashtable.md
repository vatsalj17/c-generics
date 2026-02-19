# Hash Table

Generic hash table with **string keys** and `void*` values.

- Collision handling: separate chaining
- Keys are copied internally (`strdup`)
- Values cleaned using user-provided cleanup function

---

## API

### `hash_table* hash_table_create(uint32_t size, hashfunction* hf, cleanupfunction* cf);`

Create a hash table.

- `size` → number of buckets
- `hf` → hash function
- `cf` → cleanup function for stored values (`NULL` → defaults to `free`)

Returns pointer to hash table.

---

### `void hash_table_destroy(hash_table* ht);`

Destroy table and free all memory.

- Frees keys
- Calls cleanup function on all values

---

### `bool hash_table_insert(hash_table* ht, const char *key, void *obj);`

Insert key-value pair.

Returns `false` if:
- key already exists
- invalid arguments

Key is copied internally.

---

### `void* hash_table_lookup(hash_table* ht, const char *key);`

Find value for key.

Returns pointer to value or `NULL` if not found.

---

### `void* hash_table_delete(hash_table* ht, const char *key);`

Remove entry.

Returns stored value (caller must free it if needed).

---

### `void hash_table_print(hash_table* ht);`

Debug print of table contents.

---

## Implementation

- Buckets stored as array of linked lists.
- Index = `hash(key) % size`
- Keys stored as duplicated strings.
- Values stored as raw pointers.
- No resizing → performance depends on hash function and table size.

---

## Notes

- Not thread-safe.
- Duplicate keys not allowed.
- Choose good hash function (djb2, FNV-1a, etc.).
