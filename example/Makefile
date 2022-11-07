all: main.out

# 指定搜索路径
CPATH = ../

main.out: main.o
	$(CC) -o $@ $^

main.o:

.PHONY:
clean:
	rm *.o *.out