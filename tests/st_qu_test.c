#include <stdio.h>

#include "../queue.h"
#include "../stack.h"

int main(void) {
	stack* s = stack_init(10, sizeof(int));
    int a = 4;
	push(s, &a);
	stack_destroy(s);
	printf("stack done\n");
	queue* q = queue_init(11, sizeof(int));
	queue_destroy(q);
	printf("queue done\n");
}
