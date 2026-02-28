#ifndef PQ_H
#define PQ_H

#include <stddef.h>
#include <stdbool.h>

typedef struct PriorityQueue priority_queue;

typedef int (*pq_cmp_fn)(const void *a, const void *b);
// > 0 => a has higher priority , < 0 => a has lower priority

priority_queue* pq_init(size_t capacity, pq_cmp_fn cmp);
bool pq_is_empty(priority_queue* pq);
void* pq_top(priority_queue* pq);
void pq_push(priority_queue* pq, void* data);
void* pq_pop(priority_queue* pq);
void pq_destroy(priority_queue* pq);

#endif
