#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../priority_queue.h"

typedef struct {
    int pid;
    int priority;
    char name[16];
} Process;

int compare_procs(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->priority - p2->priority; 
}

// comparison for simple integers
int compare_ints(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

void test_basic_ops() {
    printf("Running Basic Ops Test... ");
    priority_queue* pq = pq_init(10, compare_ints);
    assert(pq_is_empty(pq));

    int values[] = {10, 20, 5, 15};
    for(int i = 0; i < 4; i++) {
        pq_push(pq, &values[i]);
    }

    assert(!pq_is_empty(pq));
    // Min-heap logic: 5 should be top
    assert(*(int*)pq_top(pq) == 5);
    
    int* popped = (int*)pq_pop(pq);
    assert(*popped == 5);
    assert(*(int*)pq_top(pq) == 10);

    pq_destroy(pq);
    printf("PASSED\n");
}

void test_complex_structs() {
    printf("Running Complex Structs Test... ");
    priority_queue* pq = pq_init(5, compare_procs);

    Process procs[] = {
        {101, 1, "bash"},
        {102, 10, "hyprland"},
        {103, 5, "nvim"},
        {104, 12, "init"}
    };

    for(int i = 0; i < 4; i++) pq_push(pq, &procs[i]);

    // Highest priority is 'init' (12)
    Process* top = (Process*)pq_pop(pq);
    assert(top->pid == 104);
    assert(strcmp(top->name, "init") == 0);

    // Next is 'hyprland' (10)
    top = (Process*)pq_pop(pq);
    assert(top->pid == 102);

    pq_destroy(pq);
    printf("PASSED\n");
}

void test_stress_and_order() {
    printf("Running Stress/Order Test... ");
    int size = 1000;
    priority_queue* pq = pq_init(size, compare_ints);
    int* data = malloc(sizeof(int) * size);

    for(int i = 0; i < size; i++) {
        data[i] = rand() % 10000;
        pq_push(pq, &data[i]);
    }

    int last_val = -1;
    for(int i = 0; i < size; i++) {
        int current = *(int*)pq_pop(pq);
        if (last_val != -1) {
            assert(current >= last_val); // ensure non-decreasing order for min-heap
        }
        last_val = current;
    }

    assert(pq_is_empty(pq));
    free(data);
    pq_destroy(pq);
    printf("PASSED\n");
}

int main() {
    test_basic_ops();
    test_complex_structs();
    test_stress_and_order();
    printf("\nAll tests passed!\n");
    return 0;
}
