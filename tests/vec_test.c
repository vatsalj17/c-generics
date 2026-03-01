#include <assert.h>
#include <stdio.h>

#include "../vector.h"

typedef struct {
	int num;
	char* str;
} custom;

int comparator(const void* a, const void* b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int main(void) {
	vector* v = vec_init(sizeof(custom), NULL);
	custom obj1 = {
		.num = 1,
		.str = "Hello",
	};
	custom obj2 = {
		.num = 2,
		.str = "hello",
	};
	custom obj3 = {
		.num = 9,
		.str = "temp",
	};
	printf("sizeof obj: %lu\n", sizeof(custom));
	vec_push_back(v, &obj1);
	vec_push_back(v, &obj2);
    vec_insert_at(v, 0, &obj3);
    vec_erase_at(v, 1);

	printf("vec size: %lu\n", vec_size(v));
	void* temp = vec_data_at(v, 1);
	// void* temp = vec_back(v);
	assert(temp);
	custom obj = *(custom*)temp;
	// vec_data_at(v, 0);
	printf("Capacity: %ld, Size: %ld\n", vec_capacity(v), vec_size(v));
	printf("object got: %d, %s\n", obj.num, obj.str);
	free(temp);
	vec_free(v);

	vector* vi = vec_init_cap(sizeof(int), 3, NULL);
	int arr[6] = {1, 2, 3, 4, 5, 6};
    int arr2[3] = {21, 22, 23};
	// for (int i = 0; i < 6; i++) vec_push_back(vi, &arr[i]);
    vec_push_many(vi, (void*)arr, 6);
    vec_insert_many(vi, 3, (void*)arr2, 3);
    printf("vector<int> vi = ");
	for (size_t i = 0; i < vec_size(vi); i++) {
        void* data = vec_data_at(vi, i);
		int num = *(int*)data;
		printf("%d ", num);
        free(data);
	}
    printf("\n");
    vec_element_swap(vi, 4, 4);
    vec_reverse(vi);
    printf("Reversed: ");
	for (size_t i = 0; i < vec_size(vi); i++) {
        void* data = vec_data_at(vi, i);
		int num = *(int*)data;
		printf("%d ", num);
        free(data);
	}
    printf("\n");
    vec_sort(vi, comparator);
    printf("Sorted: ");
	for (size_t i = 0; i < vec_size(vi); i++) {
        void* data = vec_data_at(vi, i);
		int num = *(int*)data;
		printf("%d ", num);
        free(data);
	}
    printf("\n");
    int a = 34;
    int i = vec_find(vi, &a, comparator);
    printf("%d found at index %d\n", a, i);
    vec_free(vi);
}
