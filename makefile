CC = gcc
CFLAGS = -Wextra -Wall
DEBUGFLAGS = -Wextra -Wall -fsanitize=address -g
OBJ_DIR = obj
TEST_DIR = tests
SRCS = hashtable.c priority_queue.c queue.c stack.c vector.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
TEST_EXES = ht_test pq_test st_qu_test vec_test

all: ht_test pq_test st_qu_test vec_test

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

ht_test: $(TEST_DIR)/ht_test.c $(OBJ_DIR)/hashtable.o
	$(CC) $(CFLAGS) -o $@ $^

pq_test: $(TEST_DIR)/pq_test.c $(OBJ_DIR)/priority_queue.o
	$(CC) $(CFLAGS) -o $@ $^

st_qu_test: $(TEST_DIR)/st_qu_test.c $(OBJ_DIR)/queue.o $(OBJ_DIR)/stack.o
	$(CC) $(CFLAGS) -o $@ $^

vec_test: $(TEST_DIR)/vec_test.c $(OBJ_DIR)/vector.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ_DIR) $(TEST_EXES)
