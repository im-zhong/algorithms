all: permutation.out

CXXFLAGS = -std=c++11 -Wall -g

permutation.out: permutation.cpp
	c++ $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.out *.out.dSYM tmp.txt

test: permutation.out
	cat in.txt | ./permutation.out > tmp.txt; \
	diff tmp.txt out.txt