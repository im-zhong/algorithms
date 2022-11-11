all: rbtree.out
CXXFLAGS = -g

rbtree.out: rbtree.o
	$(CXX) -o $@ $^

rbtree.o:

.PHONY: clean
clean:
	rm *.o *.out
	