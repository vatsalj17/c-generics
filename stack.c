#include "stack.h"

#include <stdio.h>
#include <string.h>

typedef struct Stack {
	void* values;
	int top;
	size_t val_size;
	size_t capacity;
} stack_t;

stack_t* stack_init(size_t capacity, size_t element_size) {
	stack_t* stack = malloc(sizeof(stack_t));
	if (stack == NULL) {
		perror("stack malloc failed");
		return NULL;
	}
	stack->capacity = capacity;
	stack->top = -1;
	stack->val_size = element_size;
	stack->values = malloc(capacity * element_size);
	if (stack->values == NULL) {
		perror("array malloc failed");
		free(stack);
		return NULL;
	}
	return stack;
}

bool stack_full(stack_t* stack) {
	return (size_t)stack->top >= stack->capacity - 1;
}

bool stack_empty(stack_t* stack) {
	return stack->top == -1;
}

void push(stack_t* stack, void* value) {
	if (stack == NULL) return;
	if (stack_full(stack)) {
		size_t new_capacity = stack->capacity * 2;
		void* temp = realloc(stack->values, new_capacity * stack->val_size);
		stack->capacity = new_capacity;
		stack->values = temp;
	}
	stack->top++;
	// stack->values[stack->top] = value;
	void* new = (char*)stack->values + (stack->top * stack->val_size); // getting the pointer to the location to insert
	memcpy(new, value, stack->val_size); // actually inserting the element
}

void pop(stack_t* stack) {
	if (stack == NULL) return;
	if (stack_empty(stack)) {
		fprintf(stderr, "Queue is empty\n");
		exit(1);
	}
	stack->top--;
}

void* stack_top(stack_t* stack) {
	if (stack_empty(stack)) return NULL;
	void* top = (char*)stack->values + (stack->top * stack->val_size);
	void* result = malloc(stack->val_size);
	memcpy(result, top, stack->val_size);
	return result;
}

size_t stack_size(stack_t* stack) {
	return stack->top + 1;
}

void stack_destroy(stack_t* stack) {
	if (stack == NULL) return;
	free(stack->values);
	free(stack);
}
