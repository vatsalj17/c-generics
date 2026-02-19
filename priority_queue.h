#ifndef PQ_H
#define PQ_H

#include <stddef.h>
#include <stdbool.h>

typedef struct PriorityQueue pq_t;

typedef int (*pq_cmp_fn)(const void *a, const void *b);
// > 0 => a has higher priority , < 0 => a has lower priority

pq_t* pq_init(size_t capacity, pq_cmp_fn cmp);
bool pq_is_empty(pq_t* pq);
void* pq_top(pq_t* pq);
void pq_push(pq_t* pq, void* data);
void* pq_pop(pq_t* pq);
void pq_destroy(pq_t* pq);

#endif
