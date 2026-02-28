#include "stack.h"

#include <stdio.h>
#include <string.h>

typedef struct Stack {
	void* values;
	int top;
	size_t val_size;
	size_t capacity;
} stack;

stack* stack_init(size_t capacity, size_t element_size) {
	stack* st = malloc(sizeof(stack));
	if (st == NULL) {
		perror("stack malloc failed");
		return NULL;
	}
	st->capacity = capacity;
	st->top = -1;
	st->val_size = element_size;
	st->values = malloc(capacity * element_size);
	if (st->values == NULL) {
		perror("array malloc failed");
		free(st);
		return NULL;
	}
	return st;
}

bool stack_full(stack* st) {
	return (size_t)st->top >= st->capacity - 1;
}

bool stack_empty(stack* st) {
	return st->top == -1;
}

void push(stack* st, void* value) {
	if (st == NULL) return;
	if (stack_full(st)) {
		size_t new_capacity = st->capacity * 2;
		void* temp = realloc(st->values, new_capacity * st->val_size);
		st->capacity = new_capacity;
		st->values = temp;
	}
	st->top++;
	// st->values[st->top] = value;
	void* new = (char*)st->values + (st->top * st->val_size); // getting the pointer to the location to insert
	memcpy(new, value, st->val_size); // actually inserting the element
}

void pop(stack* st) {
	if (st == NULL) return;
	if (stack_empty(st)) {
		fprintf(stderr, "Queue is empty\n");
		exit(1);
	}
	st->top--;
}

void* stack_top(stack* st) {
	if (stack_empty(st)) return NULL;
	void* top = (char*)st->values + (st->top * st->val_size);
	void* result = malloc(st->val_size);
	memcpy(result, top, st->val_size);
	return result;
}

size_t stack_size(stack* st) {
	return st->top + 1;
}

void stack_destroy(stack* st) {
	if (st == NULL) return;
	free(st->values);
	free(st);
}
