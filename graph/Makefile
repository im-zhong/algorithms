all: main.out

# 指定搜索路径
CPATH = ../
CFLAGS = -g
CXXFLAGS = -g

main.out: main.o
	$(CC) -o $@ $^

.PHONY: test
test: test_spare_graph test_dense_graph
	./test_spare_graph.out; \
	./test_dense_graph.out

.PHONY: test_spare_graph
test_spare_graph: test_spare_graph.out
test_spare_graph.out: test_spare_graph.o
	$(CXX) -o $@ $^

.PHONY: test_dense_graph
test_dense_graph: test_dense_graph.out
test_dense_graph.out: test_dense_graph.o
	$(CXX) -o $@ $^

# make dot 可以把所有以dot为后缀的文件统统生成png图片
.PHONY: dot
dot:
	ls | egrep '*.dot$' | xargs -n 1 dot -Tpng -O

main.o:
test_dense_graph.o: dense_graph.h
test_spare_graph.o: spare_graph.h

.PHONY: clean
clean:
	rm *.o *.out *.dot *.png
