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
} queue_t;

queue_t* queue_init(size_t capacity, size_t element_size) {
	queue_t* queue = malloc(sizeof(queue_t));
	if (queue == NULL) {
		perror("queue malloc failed");
		return NULL;
	}
	queue->capacity = capacity;
	queue->head = 0;
	queue->tail = 0;
	queue->entries = 0;
	queue->val_size = element_size;
	queue->values = malloc(capacity * element_size);
	if (queue->values == NULL) {
		perror("array malloc failed");
		free(queue);
		return NULL;
	}
	return queue;
}

bool queue_full(queue_t* queue) {
	return queue->entries == queue->capacity;
}

bool queue_empty(queue_t* queue) {
	return queue->entries == 0;
}

void enqueue(queue_t* queue, void* value) {
	if (!queue) return;
	if (queue_full(queue)) {
		fprintf(stderr, "Queue is full\n");
		exit(1);
	}
	// queue->values[queue->tail] = value;
	void* new = (char*)queue->values + (queue->tail * queue->val_size);
	memcpy(new, value, queue->val_size);
	queue->tail = (queue->tail + 1) % queue->capacity;
	queue->entries++;
}

void dequeue(queue_t* queue) {
	if (!queue) return;
	if (queue_empty(queue)) {
		fprintf(stderr, "Queue is empty\n");
		exit(1);
	}
	queue->head = (queue->head + 1) % queue->capacity;
	queue->entries--;
}

void* queue_front(queue_t* queue) {
	if (!queue || queue_empty(queue)) return NULL;
	void* result = malloc(queue->val_size);
	void* src = (char*)queue->values + (queue->head * queue->val_size);
	memcpy(result, src, queue->val_size);
	return result;
}

void* queue_back(queue_t* queue) {
	if (!queue || queue_empty(queue)) return NULL;
	void* result = malloc(queue->val_size);
	size_t index = (queue->tail + queue->capacity - 1) % queue->capacity;
	void* src = (char*)queue->values + (index * queue->val_size);
	memcpy(result, src, queue->val_size);
	return result;
}

size_t queue_size(queue_t* queue) {
	return queue->entries;
}

void queue_destroy(queue_t* queue) {
	if (!queue) return;
	free(queue->values);
	free(queue);
}
