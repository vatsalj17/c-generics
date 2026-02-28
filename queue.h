#ifndef QUEUE_H
#define QUEUE_H

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Queue queue;

queue* queue_init(size_t capacity, size_t element_size);
bool queue_full(queue* queue);
bool queue_empty(queue* queue);
void enqueue(queue* queue, void* value);
void dequeue(queue* queue);
void* queue_front(queue* queue);
void* queue_back(queue* queue);
size_t queue_size(queue* queue);
void queue_destroy(queue* queue);

#endif
