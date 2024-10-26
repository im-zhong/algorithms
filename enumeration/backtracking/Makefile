all: eight_queen.out

CXXFLAGS = -std=c++11 -Wall -g

eight_queen.out: eight_queen.cpp
	c++ $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.out *.out.dSYM tmp.txt

test: eight_queen.out
	cat in.txt | ./eight_queen.out > tmp.txt; \
	diff tmp.txt out.txt