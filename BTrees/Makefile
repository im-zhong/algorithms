all: main.out
CXXFLAGS += -g

debug: CXXFLAGS += -g
debug: all

# $@ = main.out
# $^ = array.o btree.o main.o
# gcc -o main.out array.o btree.o main.o
main.out: array.o btree.o main.o
	$(CXX) -o $@ $^

.PHONY: test
test: test.out

test.out: array.o btree.o test.o
	$(CXX) -o $@ $^

array.o:
btree.o:
main.o:
test.o:

.PHONY: clean
clean:
	rm *.o *.out *.dot *.png
