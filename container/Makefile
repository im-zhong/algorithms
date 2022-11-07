all: main.out
CXXFLAGS += -g

debug: CXXFLAGS += -g
debug: all

.PHONY: container_of
container_of: container_of.out
container_of.out: container_of.o
	$(CXX) -o $@ $^

.PHONY: test
test: test.out
test.out: test.o
	$(CXX) -o $@ $^

test.o:

.PHONY: clean
clean:
	rm *.o *.out