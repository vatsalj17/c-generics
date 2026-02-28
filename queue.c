#include "queue.h"

#include <stdio.h>
#include <string.h>

typedef struct Queue {
	void* values;
	size_t val_size;
	size_t head;
	size_t tail;
	size_t entries;
	size_t capacity;
} queue;

queue* queue_init(size_t capacity, size_t element_size) {
	queue* q = malloc(sizeof(queue));
	if (q == NULL) {
		perror("queue malloc failed");
		return NULL;
	}
	q->capacity = capacity;
	q->head = 0;
	q->tail = 0;
	q->entries = 0;
	q->val_size = element_size;
	q->values = malloc(capacity * element_size);
	if (q->values == NULL) {
		perror("array malloc failed");
		free(q);
		return NULL;
	}
	return q;
}

bool queue_full(queue* q) {
	return q->entries == q->capacity;
}

bool queue_empty(queue* q) {
	return q->entries == 0;
}

void enqueue(queue* q, void* value) {
	if (!q) return;
	if (queue_full(q)) {
		fprintf(stderr, "Queue is full\n");
		exit(1);
	}
	// q->values[q->tail] = value;
	void* new = (char*)q->values + (q->tail * q->val_size);
	memcpy(new, value, q->val_size);
	q->tail = (q->tail + 1) % q->capacity;
	q->entries++;
}

void dequeue(queue* q) {
	if (!q) return;
	if (queue_empty(q)) {
		fprintf(stderr, "Queue is empty\n");
		exit(1);
	}
	q->head = (q->head + 1) % q->capacity;
	q->entries--;
}

void* queue_front(queue* q) {
	if (!q || queue_empty(q)) return NULL;
	void* result = malloc(q->val_size);
	void* src = (char*)q->values + (q->head * q->val_size);
	memcpy(result, src, q->val_size);
	return result;
}

void* queue_back(queue* q) {
	if (!q || queue_empty(q)) return NULL;
	void* result = malloc(q->val_size);
	size_t index = (q->tail + q->capacity - 1) % q->capacity;
	void* src = (char*)q->values + (index * q->val_size);
	memcpy(result, src, q->val_size);
	return result;
}

size_t queue_size(queue* q) {
	return q->entries;
}

void queue_destroy(queue* q) {
	if (!q) return;
	free(q->values);
	free(q);
}
