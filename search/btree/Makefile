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
test: test_array test_btree
	./test_array.out; \
	./test_btree.out 

test.out: array.o btree.o test.o
	$(CXX) -o $@ $^

.PHONY: test_array
test_array: test_array.out
test_array.out: array.o test_array.o
	$(CXX) -o $@ $^

.PHONY: test_btree
test_btree: test_btree.out
test_btree.out: array.o btree.o test_btree.o
	$(CXX) -o $@ $^

array.o:
btree.o:
main.o:
test_array.o:
test_btree.o:

.PHONY: clean
clean:
	rm *.o *.out *.dot *.png
