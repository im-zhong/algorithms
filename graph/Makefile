all: main.out

# 指定搜索路径
CPATH = ../

main.out: main.o
	$(CC) -o $@ $^

.PHONY: test
test: test.out

test.out: test.o
	$(CC) -o $@ $^

main.o:
test.o:

.PHONY: clean
clean:
	rm *.o *.out
