#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct Vector vector_t;
typedef void cleanupfunction(void*);

// constructors
vector_t* vec_init(size_t val_size);
vector_t* vec_init_cap(size_t val_size, size_t capacity);
vector_t* vec_clone(vector_t* vec);

// insert
void vec_push_back(vector_t* vec, void* element);
void vec_insert_at(vector_t* vec, size_t index, void* element);
void vec_push_many(vector_t* vec, void* data, size_t count);
void vec_insert_many(vector_t* vec, size_t index, void* data, size_t count);

// delete
void vec_pop_back(vector_t* vec);
void vec_erase_at(vector_t* vec, size_t index);

// access
void* vec_data_at(vector_t* vec, size_t index);
void* vec_front(vector_t* vec);
void* vec_back(vector_t* vec);
void* get_ptr(vector_t* vec, size_t index);
ssize_t vec_find(vector_t* vec, void* element, int (*cmp)(const void*, const void*));

// update
void vec_reserve(vector_t* vec, size_t new_cap);
void vec_clear(vector_t* vec);
void vec_shrink_to_fit(vector_t* vec);
void vec_element_swap(vector_t* vec, size_t a_index, size_t b_index);
void vec_reverse(vector_t* vec);
void vec_sort(vector_t* vec, int (*cmp)(const void*, const void*));

// property
size_t vec_size(vector_t* vec);
size_t vec_capacity(vector_t* vec);

// destructor
void vec_free(vector_t* vec, cleanupfunction cf);

#endif
