all: test.out

CFLAGS = -g

test.out: test.o
	$(CC) -o $@ $^; \
	./test.out

test.o: sort.h

.PHONY: clean
clean:
	rm *.o *.out