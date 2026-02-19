#ifndef QUEUE_H
#define QUEUE_H

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Queue queue_t;

queue_t* queue_init(size_t capacity, size_t element_size);
bool queue_full(queue_t* queue);
bool queue_empty(queue_t* queue);
void enqueue(queue_t* queue, void* value);
void dequeue(queue_t* queue);
void* queue_front(queue_t* queue);
void* queue_back(queue_t* queue);
size_t queue_size(queue_t* queue);
void queue_destroy(queue_t* queue);

#endif
