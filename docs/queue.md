# Queue

Generic FIFO queue implemented as a circular buffer.

Elements are **copied into the queue**.

---

## How It Works

- Uses circular buffer (`head`, `tail`, `entries`).
- Fixed capacity (no auto-resize).
- Insert at tail, remove from head.
- Not thread-safe.

---

## API

### `queue_t* queue_init(size_t capacity, size_t element_size);`
Create queue.

---

### `bool queue_full(queue_t* queue);`
Return true if queue is full.

### `bool queue_empty(queue_t* queue);`
Return true if queue is empty.

---

### `void enqueue(queue_t* queue, void* value);`
Insert element at back.

Program exits if queue is full.

---

### `void dequeue(queue_t* queue);`
Remove front element.

Program exits if queue is empty.

---

### `void* queue_front(queue_t* queue);`
Return **malloc’d copy** of front element.

### `void* queue_back(queue_t* queue);`
Return **malloc’d copy** of last element.

Caller must free returned memory.

---

### `size_t queue_size(queue_t* queue);`
Number of elements.

---

### `void queue_destroy(queue_t* queue);`
Free internal memory.

---

## Example

```c 
queue_t* q = queue_init(8, sizeof(int));

int x = 5;
enqueue(q, &x);

int* f = queue_front(q);
printf("%d\n", *f);
free(f);

dequeue(q);
queue_destroy(q);
````

---

## Notes

* Fixed capacity → check `queue_full()` before enqueue.
* Returned elements are copies.
* No custom cleanup on destroy.
