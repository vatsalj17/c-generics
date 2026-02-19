#ifndef STACK_H
#define STACK_H

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Stack stack_t;

stack_t* stack_init(size_t capacity, size_t element_size);
bool stack_full(stack_t* stack);
bool stack_empty(stack_t* stack);
void push(stack_t* stack, void* value);
void pop(stack_t* stack);
void* stack_top(stack_t* stack);
size_t stack_size(stack_t* stack);
void stack_destroy(stack_t* stack);

#endif
