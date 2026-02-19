# Stack

Generic LIFO stack storing fixed-size elements in contiguous memory.

Elements are **copied into the stack**. Capacity grows automatically.

---

## How It Works

- Internally stored as dynamic array.
- Push copies element into stack.
- Capacity doubles when full.
- Not thread-safe.

---

## API

### `stack_t* stack_init(size_t capacity, size_t element_size);`
Create stack with initial capacity.

---

### `bool stack_full(stack_t* stack);`
Return true if stack is full.

### `bool stack_empty(stack_t* stack);`
Return true if stack is empty.

---

### `void push(stack_t* stack, void* value);`
Push element onto stack.
Automatically resizes if full.

---

### `void pop(stack_t* stack);`
Remove top element.

Program exits if stack is empty.

---

### `void* stack_top(stack_t* stack);`
Return **malloc’d copy** of top element.
Caller must free it.

---

### `size_t stack_size(stack_t* stack);`
Return number of elements.

---

### `void stack_destroy(stack_t* stack);`
Free internal memory.

---

## Example

```c 
stack_t* s = stack_init(8, sizeof(int));

int x = 10;
push(s, &x);

int* t = stack_top(s);
printf("%d\n", *t);
free(t);

pop(s);
stack_destroy(s);
````

---

## Notes

* Elements must be copyable.
* `stack_top()` allocates memory.
* No custom cleanup on destroy.
