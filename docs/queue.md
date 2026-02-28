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

### `queue* queue_init(size_t capacity, size_t element_size);`
Create queue.

---

### `bool queue_full(queue* q);`
Return true if queue is full.

### `bool queue_empty(queue* q);`
Return true if queue is empty.

---

### `void enqueue(queue* q, void* value);`
Insert element at back.

Program exits if queue is full.

---

### `void dequeue(queue* q);`
Remove front element.

Program exits if queue is empty.

---

### `void* queue_front(queue* q);`
Return **malloc’d copy** of front element.

### `void* queue_back(queue* q);`
Return **malloc’d copy** of last element.

Caller must free returned memory.

---

### `size_t queue_size(queue* q);`
Number of elements.

---

### `void queue_destroy(queue* q);`
Free internal memory.

---

## Example

```c 
queue* q = queue_init(8, sizeof(int));

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
