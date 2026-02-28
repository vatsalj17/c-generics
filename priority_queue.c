#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct PriorityQueue {
	void** data;
	size_t size;
	size_t capacity;
	pq_cmp_fn cmp;
} priority_queue;

static void swap(void** a, void** b) {
	void* temp = *a;
	*a = *b;
	*b = temp;
}

priority_queue* pq_init(size_t capacity, pq_cmp_fn cmp) {
	if (capacity == 0) return NULL;
	priority_queue* new = malloc(sizeof(priority_queue));
	if (!new) return NULL;
	new->cmp = cmp;
	new->capacity = capacity;
	new->size = 0;
	void** arr = calloc(capacity, sizeof(void*));
	if (!arr) {
		free(new);
		return NULL;
	}
	new->data = arr;
	return new;
}

void pq_push(priority_queue* pq, void* data) {
	if (!pq) return;
	if (pq->size == pq->capacity) {
		size_t new_cap = pq->capacity * 2;
		void** arr = realloc(pq->data, new_cap * sizeof(void*));
		if (!arr) {
			fprintf(stderr, "realloc failed in pq_push\n");
			exit(1);
			// return;
		}
		pq->data = arr;
		pq->capacity = new_cap;
	}
	size_t index = pq->size++;
	pq->data[index] = data;
	while (index > 0) {
		size_t parent = (index - 1) / 2;
		if (pq->cmp(pq->data[index], pq->data[parent]) > 0) {
			swap(&pq->data[index], &pq->data[parent]);
			index = parent;
		} else {
			break;
		}
	}
}

void* pq_top(priority_queue* pq) {
	if (!pq || pq_is_empty(pq)) return NULL;
	return pq->data[0];
}

void* pq_pop(priority_queue* pq) {
	if (!pq || pq_is_empty(pq)) return NULL;
	void* result = pq->data[0];
	pq->data[0] = pq->data[--pq->size];
	size_t i = 0;
	while (i < pq->size) {
		size_t left = 2 * i + 1;
		size_t right = 2 * i + 2;
		size_t largest = i;
		if (left < pq->size && pq->cmp(pq->data[largest], pq->data[left]) < 0) {
			largest = left;
		}
		if (right < pq->size && pq->cmp(pq->data[largest], pq->data[right]) < 0) {
			largest = right;
		}
		if (largest != i) {
			swap(&pq->data[i], &pq->data[largest]);
			i = largest;
		} else {
			break;
		}
	}
	return result;
}

void pq_destroy(priority_queue* pq) {
	if (!pq) return;
	free(pq->data);
	free(pq);
}

bool pq_is_empty(priority_queue* pq) {
	return pq->size == 0;
}
