#include "vector.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vector {
	void* values;
	size_t val_size;
	size_t capacity;
	size_t size;
    cleanupfunction* cf;
} vector;

vector* vec_init(size_t val_size, cleanupfunction* cf) {
	vector* vec = malloc(sizeof(vector));
	if (!vec) return NULL;
	vec->val_size = val_size;
	vec->size = 0;
	vec->capacity = 0;
    vec->cf = cf;
	return vec;
}

vector* vec_init_cap(size_t val_size, size_t capacity, cleanupfunction* cf) {
	vector* vec = malloc(sizeof(vector));
	if (!vec) return NULL;
	vec->val_size = val_size;
	vec->size = 0;
	vec->capacity = capacity;
    vec->cf = cf;
	void* temp = malloc(val_size * capacity);
	if (temp == NULL) {
		fprintf(stderr, "vec_init_cap: malloc failed in val intialization\n");
		return NULL;
	}
	vec->values = temp;
	return vec;
}

vector* vec_clone(vector* vec) {
	vector* new = vec_init_cap(vec->val_size, vec->capacity, vec->cf);
	size_t bytes_to_copy = vec->size * vec->val_size;
	new->size = vec->size;
	memcpy(new->values, vec->values, bytes_to_copy);
	return new;
}

static void vec_realloc(vector* vec) {
	assert(vec->capacity > 0);
	size_t new_cap = vec->capacity * 2;
	void* temp = realloc(vec->values, new_cap * vec->val_size);
	if (temp == NULL) {
		perror("vec_realloc");
		abort();
	}
	vec->values = temp;
	vec->capacity = new_cap;
}

static void vec_realloc_cap(vector* vec, size_t new_cap) {
    void *temp = NULL;
    if (vec->capacity == 0) {
        temp = malloc(new_cap * vec->val_size);
    } else {
        temp = realloc(vec->values, new_cap * vec->val_size);
    }
	if (temp == NULL) {
		perror("vec_realloc_cap");
		abort();
	}
	vec->values = temp;
	vec->capacity = new_cap;
}

static int is_full(vector* vec) {
	if (vec->size >= vec->capacity)
		return 1;
	else
		return 0;
}

void vec_push_back(vector* vec, void* element) {
	if (vec->capacity == 0) {
		vec->capacity = 1;
		vec->values = malloc(vec->val_size * vec->capacity);
		if (vec->values == NULL) {
			perror("vec_push_back");
			abort();
		}
		memcpy(vec->values, element, vec->val_size);
		vec->size = 1;
		return;
	}
	if (is_full(vec)) vec_realloc(vec);
	void* idx_ptr = get_ptr(vec, vec->size);
	memcpy(idx_ptr, element, vec->val_size);
	vec->size++;
}

void vec_insert_at(vector* vec, size_t index, void* element) {
	assert(index <= vec->size);
	if (index == vec->size) {
		vec_push_back(vec, element);
		return;
	}
	if (is_full(vec)) vec_realloc(vec);
	void* src = get_ptr(vec, index);
	void* dest = get_ptr(vec, index + 1);
	size_t bytes_to_move = (vec->size - index + 1) * vec->val_size;
	memmove(dest, src, bytes_to_move);
	memcpy(src, element, vec->val_size);
	vec->size++;
}

void vec_push_many(vector* vec, void* data, size_t count) {
	size_t new_cap = vec->size + count;
	if (vec->capacity == 0) {
		size_t space = new_cap * vec->val_size;
		void* new = malloc(space);
		if (!new) {
			perror("vec_push_many");
			abort();
		}
		vec->values = new;
		vec->size = count;
		vec->capacity = count;
		memcpy(vec->values, data, space);
		return;
	}
	if (new_cap > vec->capacity) {
		vec_realloc_cap(vec, new_cap);
	}
	void* idx_ptr = get_ptr(vec, vec->size);
	memcpy(idx_ptr, data, vec->val_size * count);
	vec->size += count;
}

void vec_insert_many(vector* vec, size_t index, void* data, size_t count) {
	if (vec->capacity == 0 || index == vec->size + 1) {
		vec_push_many(vec, data, count);
		return;
	}
	size_t new_cap = count + vec->size;
	if (new_cap >= vec->capacity) {
		vec_realloc_cap(vec, new_cap + 1);
	}
	void* src = get_ptr(vec, index);
	void* dest = get_ptr(vec, index + count);
	size_t bytes_to_move = (vec->size - index + 1) * vec->val_size;
	memmove(dest, src, bytes_to_move);
	memcpy(src, data, vec->val_size * count);
	vec->size += count;
}

void vec_pop_back(vector* vec) {
	assert(vec->size);
	vec->size--;
}

void vec_erase_at(vector* vec, size_t index) {
	assert(index < vec->size);
	if (index == vec->size - 1) {
		vec_pop_back(vec);
		return;
	}
	void* src = get_ptr(vec, index + 1);
	void* dest = get_ptr(vec, index);
	size_t bytes_to_move = (vec->size - index) * vec->val_size;
	memmove(dest, src, bytes_to_move);
	vec->size--;
}

void* get_ptr(vector* vec, size_t index) {
	if (index > vec->size) return NULL;
	return (char*)vec->values + (index * vec->val_size);
}

void* vec_data_at(vector* vec, size_t index) {
	if (index >= vec->size) return NULL;
	void* new = malloc(vec->val_size);
	if (!new) {
		perror("vec_data_at");
		exit(1);
	}
	void* data = (char*)vec->values + (index * vec->val_size);
	memcpy(new, data, vec->val_size);
	return new;
}

void* vec_front(vector* vec) {
	if (vec->size == 0) return NULL;
	return vec_data_at(vec, 0);
}

void* vec_back(vector* vec) {
	if (vec->size == 0) return NULL;
	return vec_data_at(vec, vec->size - 1);
}

ssize_t vec_find(vector* vec, void* element, int (*cmp)(const void*, const void*)) {
	for (size_t i = 0; i < vec->size; i++) {
		void* data = get_ptr(vec, i);
		if (cmp(data, element) == 0) return i;
	}
	return -1;
}

void vec_clear(vector* vec) {
	vec->size = 0;
}

void vec_reserve(vector* vec, size_t new_cap) {
	if (new_cap < vec->capacity) {
		printf("vec_reserve: Capacity is already sufficient\n");
		return;
	}
	vec_realloc_cap(vec, new_cap);
}

void vec_shrink_to_fit(vector* vec) {
	assert(vec->size <= vec->capacity);
	if (vec->size == vec->capacity) return;
	vec_realloc_cap(vec, vec->size);
}

void vec_element_swap(vector* vec, size_t a_index, size_t b_index) {
	void* temp = malloc(vec->val_size);
	if (!temp) {
		fprintf(stderr, "vec_element_swap: malloc failed\n");
		abort();
	}
	void* a = get_ptr(vec, a_index);
	void* b = get_ptr(vec, b_index);
	memcpy(temp, a, vec->val_size);
	memcpy(a, b, vec->val_size);
	memcpy(b, temp, vec->val_size);
	free(temp);
}

void vec_reverse(vector* vec) {
	size_t mid = vec->size / 2;
	size_t first_idx = 0;
	size_t last_idx = vec->size - 1;
	while (first_idx < mid) {
		vec_element_swap(vec, first_idx, last_idx);
		first_idx++;
		last_idx--;
	}
}

void vec_sort(vector* vec, int (*cmp)(const void*, const void*)) {
	qsort(vec->values, vec->size, vec->val_size, cmp);
}

size_t vec_size(vector* vec) {
	return vec->size;
}

size_t vec_capacity(vector* vec) {
	return vec->capacity;
}

void vec_free(vector* vec) {
    free(vec->values);
    free(vec);
}
