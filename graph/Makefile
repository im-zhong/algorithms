all: main.out

# 指定搜索路径
CPATH = ../
CFLAGS = -g

main.out: main.o
	$(CC) -o $@ $^

.PHONY: test
test: test_spare_graph test_dense_graph
	./test_spare_graph.out; \
	./test_dense_graph.out

.PHONY: test_spare_graph
test_spare_graph: test_spare_graph.out
test_spare_graph.out: test_spare_graph.o
	$(CC) -o $@ $^

.PHONY: test_dense_graph
test_dense_graph: test_dense_graph.out
test_dense_graph.out: test_dense_graph.o
	$(CC) -o $@ $^

main.o:
test_dense_graph.o:
test_spare_graph.o:

.PHONY: clean
clean:
	rm *.o *.out
