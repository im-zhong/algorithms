all: subset_generation.out

CXXFLAGS = -std=c++11 -Wall -g

subset_generation.out: subset_generation.cpp
	c++ $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.out *.out.dSYM tmp.txt

test: subset_generation.out
	cat in.txt | ./subset_generation.out > tmp.txt; \
	diff tmp.txt out.txt