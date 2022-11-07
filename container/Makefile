all: main.out
CXXFLAGS += -g

debug: CXXFLAGS += -g
debug: all

.PHONY: test
test: test.out
test.out: test.o
	$(CC) -o $@ $^

test.o:

.PHONY: clean
clean:
	rm *.o *.out