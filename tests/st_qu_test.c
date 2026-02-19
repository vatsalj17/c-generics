#include <stdio.h>

#include "../queue.h"
#include "../stack.h"

int main(void) {
	stack_t* s = stack_init(10, sizeof(int));
    int a = 4;
	push(s, &a);
	stack_destroy(s);
	printf("got it\n");
	queue_t* q = queue_init(11, sizeof(int));
	queue_destroy(q);
}
