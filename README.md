# c-generics

A small, dependency-free library of **generic data structures for C**.

No macros. No templates. No magic. Just `void*`, `memcpy` and `memmove`

---

## Features

- Generic containers using type-erasure
- No external dependencies
- Predictable memory behavior
- Small, readable codebase
- Useful for learning internals or real systems projects

---

## Data Structures

| Structure | Description |
|------------|-------------|
| **[Vector](docs/vector.md)** | Dynamic array storing contiguous copies of elements. Supports bulk insert, sort, reverse. |
| **[Hash Table](docs/hashtable.md)** | Separate chaining hash table with string keys and `void*` values. |
| **[Priority Queue](docs/priority_queue.md)** | Binary heap storing pointers with custom comparator. |
| **[Queue](docs/queue.md)** | Fixed-capacity circular buffer FIFO queue. |
| **[Stack](docs/stack.md)** | Dynamic array-based LIFO stack. |

Each structure has its own short documentation inside `docs/`.

---

## Design Philosophy

C has no true generics, so this library uses **type erasure**.

You pass `sizeof(type)` when creating containers, and the library stores raw bytes in contiguous memory using pointer arithmetic:

```
values + index * element_size
````

### Memory Rules

1. **Copy vs Pointer Storage**
   - `vector`, `stack`, `queue` → store copies of elements
   - `priority_queue`, `hashtable` values → store pointers

2. **Some accessors allocate memory**
   Functions like:
   - `vec_front()`
   - `queue_front()`
   - `stack_top()`

   return heap copies → **caller must free them**.

3. **Custom Cleanup**
   Some structures allow cleanup callbacks to safely free complex objects.

---

## Quick Start

### Clone

```bash
git clone https://github.com/vatsalj17/c-generics.git
cd c-generics
````

### Build Tests

```bash
make all
./vec_test
./ht_test
./pq_test
./st_qu_test
```

### Clean

```bash
make clean
```

---

## Example

```c
#include "vector.h"
#include <stdio.h>

int main() {
    vector_t* v = vec_init(sizeof(int));

    int x = 42;
    vec_push_back(v, &x);

    int* y = vec_front(v);
    printf("%d\n", *y);
    free(y);

    vec_free(v, NULL);
}
```

---

##  Safety Notes

* Not thread-safe.
* Bounds checks mostly via `assert`.
* Some containers don’t auto-resize (queue).
* Pointer lifetimes are caller’s responsibility.
* Designed for learning and controlled environments.
