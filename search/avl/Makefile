all: test.out

# 指定搜索路径
export CPATH = ../../

# 我想达到一种效果 make, make debug, make test, 但是make test一定是debug的
debug test: CFLAGS += -g
debug: all

.PHONY: test
test: test.out
	./test.out
test.out: avl.o test.o
	$(CC) -o $@ $^
avl.o: avl.h
test.o: avl.h

.PHONY: dot
dot:
	ls | egrep '*.dot$$' | xargs -n 1 dot -Tpng -O

.PHONY: clean
clean:
	rm *.o *.out *.dot *.png
