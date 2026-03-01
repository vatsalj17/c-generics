#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct Vector vector;
typedef void cleanupfunction(void*);

// constructors
vector* vec_init(size_t val_size, cleanupfunction* cf);
vector* vec_init_cap(size_t val_size, size_t capacity, cleanupfunction* cf);
vector* vec_clone(vector* vec);

// insert
void vec_push_back(vector* vec, void* element);
void vec_insert_at(vector* vec, size_t index, void* element);
void vec_push_many(vector* vec, void* data, size_t count);
void vec_insert_many(vector* vec, size_t index, void* data, size_t count);

// delete
void vec_pop_back(vector* vec);
void vec_erase_at(vector* vec, size_t index);

// access
void* vec_data_at(vector* vec, size_t index); /* caller must free */
void* vec_front(vector* vec); /* caller must free */
void* vec_back(vector* vec); /* caller must free */
void* get_ptr(vector* vec, size_t index);
ssize_t vec_find(vector* vec, void* element, int (*cmp)(const void*, const void*));

// update
void vec_reserve(vector* vec, size_t new_cap);
void vec_clear(vector* vec);
void vec_shrink_to_fit(vector* vec);
void vec_element_swap(vector* vec, size_t a_index, size_t b_index);
void vec_reverse(vector* vec);
void vec_sort(vector* vec, int (*cmp)(const void*, const void*));

// property
size_t vec_size(vector* vec);
size_t vec_capacity(vector* vec);

// destructor
void vec_free(vector* vec);

#endif
