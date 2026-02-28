#ifndef STACK_H
#define STACK_H

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Stack stack;

stack* stack_init(size_t capacity, size_t element_size);
bool stack_full(stack* stack);
bool stack_empty(stack* stack);
void push(stack* stack, void* value);
void pop(stack* stack);
void* stack_top(stack* stack);
size_t stack_size(stack* stack);
void stack_destroy(stack* stack);

#endif
