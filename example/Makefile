all: main.out

# 指定搜索路径
# 在这里指定的为什么没有用呢??
# 但是这样做会修改你的CPATH环境变量 还是非常不好
# 还是得换成conan + cmake啊
# 或者 conan + cmake + ninja
export CPATH = ../

main.out: main.o
	$(CC) -o $@ $^

# 我懂了 这就意味着container目录的文件需要进行编译
# 然后我在这里进行引用
# 这非常的麻烦
# 不如换成CMake
main.o:

.PHONY: clean
clean:
	rm *.o *.out
