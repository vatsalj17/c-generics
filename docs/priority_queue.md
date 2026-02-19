# Priority Queue

Generic priority queue implemented as a **binary heap**.

Stores `void*` elements ordered by a user-provided comparator.

---

## How It Works

- Internally stored as a binary heap in a dynamic array.
- Parent/child ordering maintained using `cmp()` function.
- Capacity doubles automatically when full.
- Queue stores **pointers only** (does not copy data).
- Not thread-safe.

---

## Comparator

```c
int cmp(const void *a, const void *b);
````

* `> 0` → `a` has higher priority
* `< 0` → `b` has higher priority

Example for max-heap of integers:

```c
int cmp_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}
```

---

## API

### `pq_t* pq_init(size_t capacity, pq_cmp_fn cmp);`

Create priority queue with initial capacity.

Returns `NULL` on failure.

---

### `void pq_push(pq_t* pq, void* data);`

Insert element into queue.

* Automatically resizes if full.
* Stores pointer only.

---

### `void* pq_top(pq_t* pq);`

Return highest-priority element without removing it.

Returns `NULL` if empty.

---

### `void* pq_pop(pq_t* pq);`

Remove and return highest-priority element.

Caller is responsible for freeing element if needed.

---

### `bool pq_is_empty(pq_t* pq);`

Return `true` if queue is empty.

---

### `void pq_destroy(pq_t* pq);`

Free internal memory.

Does **not** free stored elements.

---

## Example

```c
int cmp_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

pq_t* pq = pq_init(8, cmp_int);

int a=5,b=10,c=3;
pq_push(pq,&a);
pq_push(pq,&b);
pq_push(pq,&c);

printf("%d\n", *(int*)pq_pop(pq)); // 10

pq_destroy(pq);
```

---

## Notes

* Elements must remain valid while in queue.
* No custom cleanup on destroy.
* Good for scheduling, Dijkstra, task queues, etc.
