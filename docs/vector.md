# Vector

Generic dynamic array storing fixed-size elements.

- Stores **copies** of elements (not pointers unless you store pointers yourself)
- Automatically resizes
- Supports bulk insert, sort, reverse, swap, etc.

---

## API

### Constructors

`vector* vec_init(size_t val_size, cleanupfunction cf);`  
Create empty vector.

`vector* vec_init_cap(size_t val_size, size_t capacity, cleanupfunction cf);`  
Create vector with initial capacity.

`vector* vec_clone(vector* vec);`  
Deep copy of vector memory.

---

### Insert

`void vec_push_back(vector* vec, void* element);`  
Append one element.

`void vec_insert_at(vector* vec, size_t index, void* element);`  
Insert at index.

`void vec_push_many(vector* vec, void* data, size_t count);`  
Append multiple elements.

`void vec_insert_many(vector* vec, size_t index, void* data, size_t count);`  
Insert multiple elements.

---

### Delete

`void vec_pop_back(vector* vec);`  
Remove last element.

`void vec_erase_at(vector* vec, size_t index);`  
Remove element at index.

---

### Access

`void* vec_data_at(vector* vec, size_t index);`  
Return **copy** of element (malloc'd).

`void* vec_front(vector* vec);`  
Copy of first element.

`void* vec_back(vector* vec);`  
Copy of last element.

`void* get_ptr(vector* vec, size_t index);`  
Pointer to internal memory (no copy).

`ssize_t vec_find(vector* vec, void* element, cmp_fn);`  
Return index or `-1`.

---

### Update / Utilities

`void vec_reserve(vector* vec, size_t new_cap);`  
Ensure capacity.

`void vec_clear(vector* vec);`  
Remove all elements.

`void vec_shrink_to_fit(vector* vec);`  
Shrink capacity to size.

`void vec_element_swap(vector* vec, size_t a, size_t b);`  
Swap elements.

`void vec_reverse(vector* vec);`  
Reverse order.

`void vec_sort(vector* vec, cmp_fn);`  
Sort using `qsort`.

---

### Properties

`size_t vec_size(vector* vec);`  
Current element count.

`size_t vec_capacity(vector* vec);`  
Current capacity.

---

### Destructor

`void vec_free(vector* vec);`  

Free vector memory.

---

## Implementation

- Stored as contiguous memory (`values` buffer).
- Doubles capacity on resize.
- Uses `memcpy`/`memmove` for insert/delete.
- Sorting via `qsort`.
- No thread safety.

---

## Notes

- `vec_data_at`, `vec_front`, `vec_back` allocate memory → caller must `free`.
- `get_ptr` returns internal pointer → invalid after resize.
- Not bounds-safe unless asserts enabled.
