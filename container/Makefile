all: main.out
CXXFLAGS += -g

debug: CXXFLAGS += -g
debug: all

.PHONY: test
test: test.out
	./test.out

test.out: test.o
	$(CXX) -o $@ $^

# 我这样写的不对 不过某个头文件改了 那么这个是不会自动重新编译的
# makefile 会根据后面的依赖决定会不会重新编译
# 所以每个.o文件后面必须跟上他依赖的头文件
# test.o:
test.o: *.h

.PHONY: clean
clean:
	rm *.o *.out