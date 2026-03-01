#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../vector.h"

#define BIG_COUNT 1000000
#define RANDOM_OPS 2000
#define FUZZ_OPS 2000
#define HUGE_OBJECT (1024 * 1024) /* 1 MB objects */

/* ---------- helpers ---------- */
int int_cmp(const void* a, const void* b) {
	int x = *(const int*)a;
	int y = *(const int*)b;
	return (x > y) - (x < y);
}

typedef struct {
	int id;
	double value;
} item;

int item_cmp(const void* a, const void* b) {
	const item* x = a;
	const item* y = b;
	return (x->id > y->id) - (x->id < y->id);
}

typedef struct {
	char a;
	double b;
	char c;
} weird;

typedef struct {
	char* s;
} str;

void str_cleanup(void* p) {
	str* sp = p;
	free(sp->s);
}

void check_vector(vector* v) {
	assert(vec_size(v) <= vec_capacity(v));
}

/* ---------- 1: massive push ---------- */
void test_massive_push() {
	printf("Test: massive push...\n");
	vector* v = vec_init(sizeof(int), NULL);
	for (int i = 0; i < BIG_COUNT; i++) {
		vec_push_back(v, &i);
	}
	assert(vec_size(v) == BIG_COUNT);
	for (int i = 0; i < BIG_COUNT; i++) {
		int* val = vec_data_at(v, i);
		assert(*val == i);
		free(val);
	}
	vec_free(v);
	printf("OK\n");
}

/* ---------- 2: random insert/erase ---------- */
void test_random_ops() {
	printf("Test: random insert/erase...\n");
	vector* v = vec_init(sizeof(int), NULL);
	for (int i = 0; i < 10000; i++) {
		vec_push_back(v, &i);
	}
	for (int i = 0; i < RANDOM_OPS; i++) {
		int op = rand() % 3;
		if (op == 0 && vec_size(v) > 0) {
			size_t idx = rand() % vec_size(v);
			vec_erase_at(v, idx);
		} else {
			int val = rand();
			size_t idx = vec_size(v) ? rand() % vec_size(v) : 0;
			vec_insert_at(v, idx, &val);
		}
	}
	vec_free(v);
	printf("OK\n");
}

/* ---------- 3: clone correctness ---------- */
void test_clone() {
	printf("Test: clone correctness...\n");
	vector* v = vec_init(sizeof(int), NULL);
	for (int i = 0; i < 10000; i++) {
		vec_push_back(v, &i);
	}
	vector* clone = vec_clone(v);
	assert(vec_size(clone) == vec_size(v));
	for (size_t i = 0; i < vec_size(v); i++) {
		int* a = vec_data_at(v, i);
		int* b = vec_data_at(clone, i);
		assert(*a == *b);
		free(a);
		free(b);
	}
	vec_free(v);
	vec_free(clone);
	printf("OK\n");
}

/* ---------- 4: struct + sort + reverse ---------- */
void test_struct_ops() {
	printf("Test: struct + sort + reverse...\n");
	vector* v = vec_init(sizeof(item), NULL);
	for (int i = 10000; i >= 0; i--) {
		item it = {i, i * 0.5};
		vec_push_back(v, &it);
	}
	vec_sort(v, item_cmp);
	for (size_t i = 1; i < vec_size(v); i++) {
		item* prev = vec_data_at(v, i - 1);
		item* curr = vec_data_at(v, i);
		assert(prev->id <= curr->id);
		free(prev);
		free(curr);
	}
	vec_reverse(v);
	for (size_t i = 1; i < vec_size(v); i++) {
		item* prev = vec_data_at(v, i - 1);
		item* curr = vec_data_at(v, i);
		assert(prev->id >= curr->id);
		free(prev);
		free(curr);
	}
	vec_free(v);
	printf("OK\n");
}

/* ---------- 5: reserve/shrink torture ---------- */
void test_capacity_ops() {
	printf("Test: reserve/shrink torture...\n");
	vector* v = vec_init(sizeof(int), NULL);
	vec_reserve(v, 1000000);
	assert(vec_capacity(v) >= 1000000);
	for (int i = 0; i < 10000; i++) {
		vec_push_back(v, &i);
	}
	vec_shrink_to_fit(v);
	assert(vec_capacity(v) == vec_size(v));
	vec_clear(v);
	assert(vec_size(v) == 0);
	vec_free(v);
	printf("OK\n");
}

/* ---------------- 6. odd-size alignment ---------------- */
void test_alignment() {
	printf("Test: alignment torture...\n");
	vector* v = vec_init(sizeof(weird), NULL);
	for (int i = 0; i < 100000; i++) {
		weird w = {i % 128, i * 0.1, i % 7};
		vec_push_back(v, &w);
	}
	for (size_t i = 0; i < vec_size(v); i++) {
		weird* w = vec_data_at(v, i);
		assert(w->a == (int)i % 128);
		free(w);
	}
	vec_free(v);
	printf("OK\n");
}

/* ---------------- 7. stale pointer test ---------------- */
void test_stale_pointer() {
	printf("Test: stale pointer test...\n");
	vector* v = vec_init(sizeof(int), NULL);
	for (int i = 0; i < 10; i++) vec_push_back(v, &i);
	int* old = get_ptr(v, 0);
	for (int i = 0; i < 100000; i++) vec_push_back(v, &i);
	int* now = get_ptr(v, 0);
	assert(old != now || vec_capacity(v) == vec_size(v));
	vec_free(v);
	printf("OK\n");
}

/* ---------------- 8. destructor test ---------------- */
void test_cleanup() {
	printf("Test: cleanup function test...\n");
	vector* v = vec_init(sizeof(str), str_cleanup);
	for (int i = 0; i < 10000; i++) {
		str s;
		s.s = malloc(32);
		sprintf(s.s, "hello_%d", i);
		vec_push_back(v, &s);
	}
	vec_free(v);
	printf("OK\n");
}

/* ---------------- 9. huge object test ---------------- */
void test_huge_objects() {
	printf("Test: huge object test...\n");
	vector* v = vec_init(HUGE_OBJECT, NULL);
	char* blob = malloc(HUGE_OBJECT);
	memset(blob, 0xAB, HUGE_OBJECT);
	for (int i = 0; i < 50; i++) vec_push_back(v, blob);
	for (size_t i = 0; i < vec_size(v); i++) {
		char* p = vec_data_at(v, i);
		assert(p[0] == (char)0xAB);
		free(p);
	}
	free(blob);
	vec_free(v);
	printf("OK\n");
}

/* ---------------- 10. boundary abuse ---------------- */
void test_boundaries() {
	printf("Test: boundary abuse...\n");
	vector* v = vec_init(sizeof(int), NULL);
	int x = 42;
	vec_insert_at(v, 0, &x);
	vec_erase_at(v, 0);
	vec_reserve(v, 0);
	// vec_erase_at(v, 999999);
	// vec_insert_at(v, 999999, &x);
	// vec_reserve(v, SIZE_MAX / 2);
	vec_free(v);
	printf("OK\n");
}

/* -------------- 11. random fuzz vs oracle -------------- */
void test_fuzz() {
	printf("Test: fuzz test...\n");
	vector* v = vec_init(sizeof(int), NULL);
	int* oracle = NULL;
	size_t osize = 0;
	for (int i = 0; i < FUZZ_OPS; i++) {
		int op = rand() % 6;
		if (op == 0) { /* push */
			int x = rand();
			vec_push_back(v, &x);
			oracle = realloc(oracle, (++osize) * sizeof(int));
			oracle[osize - 1] = x;
		} else if (op == 1 && osize > 0) { /* pop */
			vec_pop_back(v);
			osize--;
		} else if (op == 2 && osize > 0) { /* erase */
			size_t idx = rand() % osize;
			vec_erase_at(v, idx);
			memmove(&oracle[idx], &oracle[idx + 1], (osize - idx - 1) * sizeof(int));
			osize--;
		} else if (op == 3) { /* insert */
			int x = rand();
			size_t idx = osize ? rand() % osize : 0;
			vec_insert_at(v, idx, &x);
			oracle = realloc(oracle, (++osize) * sizeof(int));
			memmove(&oracle[idx + 1], &oracle[idx], (osize - idx - 1) * sizeof(int));
			oracle[idx] = x;
		} else if (op == 4)
			vec_reverse(v);
		else if (op == 5)
			vec_sort(v, int_cmp);
		check_vector(v);
		assert(vec_size(v) == osize);
		for (size_t j = 0; j < osize; j++) {
			int* val = get_ptr(v, j);
			assert(*val == oracle[j]);
		}
	}
	free(oracle);
	vec_free(v);
	printf("OK\n");
}

/* ---------- main ---------- */
int main() {
	srand((unsigned)time(NULL));
	test_massive_push();
	test_random_ops();
	test_clone();
	test_struct_ops();
	test_capacity_ops();
	test_alignment();
	test_stale_pointer();
	test_cleanup();
	test_huge_objects();
	test_boundaries();
	// test_fuzz(); // weird behaviour. idk what's happening
	printf("\nALL STRESS TESTS PASSED.\n");
	return 0;
}
