// 2022/12/9
// zhangzhong
// huffman code

#include "string/huffman.h"
#include "container/bheap.h"
#include "container/vector.h"
#include "util/bitbuf.h"
#include "util/c_string.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

// 我们可以把打开文件 和 向文件中写入内容都放在一起
// 这样应用程序部分写起来就非常简单了

// 用 rewind 重置文件offset
#define ASCII_SIZE 256

typedef struct huffman_node_t {
    struct huffman_node_t* left;
    struct huffman_node_t* right;
    size_t freq;
    int c;
} huffman_node_t;

static huffman_node_t* make_huffman_node(int c, int freq) {
    huffman_node_t* node = (huffman_node_t*)malloc(sizeof(huffman_node_t));
    node->left = NULL;
    node->right = NULL;
    node->freq = freq;
    node->c = c;
    return node;
}

bool huffman_cmp(const void* n1, const void* n2) {
    return ((huffman_node_t*)n1)->freq < ((huffman_node_t*)n2)->freq;
}

static bool is_leaf(huffman_node_t* node) {
    return node->left == NULL && node->right == NULL;
}

// open file for read
// 如果打开失败 就退出程序即可
FILE* open_file_for_read(char* filename) {
    FILE* stream = fopen(filename, "r");
    if (stream == NULL) {
        printf("open %s failed.\n", filename);
        exit(EXIT_FAILURE);
    }
    return stream;
}

FILE* open_file_for_write(char* filename) {
    FILE* stream = fopen(filename, "w");
    if (stream == NULL) {
        printf("open %s failed.\n", filename);
        exit(EXIT_FAILURE);
    }
    return stream;
}

// 不对不对 我们这里应该输出一个二进制bit
// 而不是一个字符 0 或者 1啊
// 这需要额外的数据结构支持
// 不行啊  我们不能直接输出一个bit呀 至少都是一个byte
// ... 怎么办
// todo: 写一个bitbuf
static void write_huffman_tree_to_file(bitbuf_t* outbf, huffman_node_t* root) {
    // 前序遍历
    if (root) {
        // 如果是leaf, 那么输出我们自己的字符串
        if (is_leaf(root)) {
            // 这是不对的
            // 我们的bitbuf必须提供一个新的接口
            // 叫做输出一个byte
            // fprintf(out, "%s", code->data);
            bitbuf_write_bit(outbf, 1);
            // 然后把叶子节点对应的字符写进去
            bitbuf_write_byte(outbf, root->c);
        } else {
            // 如果我们是内部节点 则写一个0
            bitbuf_write_bit(outbf, 0);
            // 我们可能有左孩子或者右孩子
            // 先遍历左子树
            // 然后递归的调用左子树和右子树即可
            write_huffman_tree_to_file(outbf, root->left);
            write_huffman_tree_to_file(outbf, root->right);
        }
    }
}

// write huffman tree to file
huffman_node_t* read_huffman_tree_from_file(bitbuf_t* inbf) {
    // 从文件中
    // 同样是通过前序遍历
    // 然后我们从bitbuf中读
    // 如果读到了一个零 表示我们当前是内部节点
    // 那么我们一定有左子树也有右子树
    int bit = 0;
    bitbuf_read_bit(inbf, &bit);
    if (bit == 0) {
        // 我们读到了内部节点
        // 每个函数都会返回一个节点
        // 那么我们只需要递归的调用自身 就可以正确的生成两个孩子节点
        huffman_node_t* node = make_huffman_node(0, 0);
        node->left = read_huffman_tree_from_file(inbf);
        node->right = read_huffman_tree_from_file(inbf);
        return node;
    } else { // NOLINT
        // 我们读到了一个叶子节点
        // 读取一个byte到
        unsigned int byte = 0;
        bitbuf_read_byte(inbf, &byte);
        huffman_node_t* leaf = make_huffman_node(byte, 0);
        return leaf;
    }
}

// 这个递归遍历可以写成一个函数 复用
// 这个没法复用 这三个函数干的是完全不一样的事情 不是统一的执行模板
// write_huffman_tree_to_file
// read_huffman_tree_from_file
// make_huffman_code_index
// 这个参数你看着不迷惑吗？？ 两个vector_t* ?? 什么鬼
void make_huffman_code_index_impl(huffman_node_t* root, vector_t* index,
                                  vector_t* code) {
    if (root) {
        // 如果是leaf, 那么输出我们自己的字符串
        if (is_leaf(root)) {
            // string_append(&index[root->c], "%s", code->data);
            // 我希望push一整个vector呢
            // 可以用copy vector 也能满足这里的需求
            index[root->c] = copy_vector(code);
        } else {
            // 我们可能有左孩子或者右孩子
            // 先遍历左子树
            // string_pushback(code, 'char c')
            // bug!
            // 不能push0和1呀 因为在字符串里面 0 表示字符串结束
            // 你push一个零 啥都没了
            // 这里还是改成使用vector吧
            vector_push_back(code, 0);
            make_huffman_code_index_impl(root->left, index, code);
            vector_pop_back(code);

            // 然后遍历右子树
            vector_push_back(code, 1);
            make_huffman_code_index_impl(root->right, index, code);
            vector_pop_back(code);
        }
    }
}

// 我们要创建一个huffman code的索引数组
void make_huffman_code_index(huffman_node_t* root, vector_t* index) {
    // 初始化索引数组
    for (int i = 0; i < ASCII_SIZE; i++) {
        index[i] = make_vector(0);
    }

    // 然后深度优先搜索这个树
    // 每当搜索到一个叶子 就把目前的string给放进去
    vector_t code = make_vector(0);
    make_huffman_code_index_impl(root, index, &code);
}

void free_huffman_tree(huffman_node_t* root) {}

// 频率用uint64_t更好 因为输入文件可能非常大
huffman_node_t* make_huffman_tree(uint64_t* freq, size_t n) {
    // 然后创建index_heap
    // 不对 huffman node 会动态的从heap中删除 然后添加进去
    // index_heap不具备这个特性
    // 不能用index heap 就得用普通的bheap
    // 对于freq中频率不为0的字符 创建一个huffman_node
    // 并且将这个huffman_node 创建最小堆
    // 那么我们必须使用 index_heap
    // 我们可以使用一个数组来保存所有需要的huffman_node

    // 然后创建huffman node数组
    // 不对呀，这个huffman node数组最多就是256个呀
    // 我就申请256个 然后只用前面的几个不就行了吗！！！
    //   huffman_node_t leaf[ASCII_SIZE];
    //   // 初始化所有的node
    //   for (int i = 0; i < ASCII_SIZE; i++) {
    //     leaf[i].left = NULL;
    //     leaf[i].right = NULL;
    //     leaf[i].freq = 0;
    //     leaf[i].c = 0;
    //   }

    // 这个包装成一个函数
    bheap_t heap;
    bheap_init(&heap, n, huffman_cmp);

    // 然后再次遍历freq 适当的填充leaf数组
    int i = 0;
    for (int c = 0; c < n; c++) {
        if (freq[c] != 0) {
            huffman_node_t* node = make_huffman_node(c, freq[c]);
            // 然后将此node 插入bheap
            bheap_push(&heap, node);
        }
    }

    // 然后我们需要构建哈夫曼树
    // 只要heap非空 就一直进行这个操作
    while (bheap_size(&heap) > 1) {

        // 当heap中只有一个节点的时候就返回

        // 弹出两个最小的节点
        // todo: 让pop同时返回存储元素的指针
        huffman_node_t* n1 = (huffman_node_t*)bheap_pop(&heap);
        huffman_node_t* n2 = (huffman_node_t*)bheap_pop(&heap);

        // 然后创建一一个新的节点
        huffman_node_t* node = make_huffman_node(0, n1->freq + n2->freq);
        node->left = n1;
        node->right = n2;
        bheap_push(&heap, node);
    }

    // bheap里面至少有一个元素
    assert(bheap_size(&heap) == 1);
    huffman_node_t* root = bheap_pop(&heap);

    return root;
}

string_t get_compress_filename(char* filename) {
    return make_string("%s.huf", filename);
}

void huffman_compress(char* filename) {
    // 第一遍读取整个字符流 直到
    FILE* in = open_file_for_read(filename);
    // 还不行 因为不是所有的节点都会有频率 不能全部放到index_heap里面
    uint64_t freq[ASCII_SIZE];
    for (int i = 0; i < ASCII_SIZE; i++) {
        freq[i] = 0;
    }
    uint64_t len = 0;
    int c = 0;
    while ((c = fgetc(in)) != EOF) {
        // 统计频率 那么请问 这个频率我记录在哪??
        // 我可以先记录在一个数组里面
        freq[c]++;
        len++;
    }

    // 这样我们就拿到了哈夫曼树的根
    // 然后我们需要前序遍历哈夫曼树 先将我们自己编码到输出文件中
    huffman_node_t* root = make_huffman_tree(freq, ASCII_SIZE);

    // todo: 我要创建一个输入的文件呀
    string_t compress_filename = get_compress_filename(filename);
    FILE* out = open_file_for_write(compress_filename.data);
    free_string(&compress_filename);

    // 向此文件中输出huffman树
    bitbuf_t* outbf = make_bitbuf(out);
    write_huffman_tree_to_file(outbf, root);

    // 然后输出字符数量
    bitbuf_write_len(outbf, len);

    // 然后根据输入字节流转换成bit输出
    // 所以我还需要一个数组用来保存每个ascii的字符对应的bit
    // 我要怎么保存呢??
    // 用链表保存bit??
    // 用string呀 方便的一批
    vector_t code_index[ASCII_SIZE];
    make_huffman_code_index(root, code_index);

    // 重置文件offset
    rewind(in);
    // 然后就是读取字节流 转换成bit流 输出就可以了
    c = 0;
    while ((c = fgetc(in)) != EOF) {
        vector_t code = code_index[c];
        // // 这个包装成一个函数 就改成 bitbuf_write_string
        // // bitbuf_read_string
        // for (int i = 0; i < code.size; i++) {
        //   bitbuf_write_bit(outbf, code.data[i]);
        // }
        bitbuf_write_vector(outbf, code, code.size);
    }
    bitbuf_flush(outbf);

    // 关闭文件与清理对象
    free_huffman_tree(root);
    free_bitbuf(outbf);
    fclose(in);
    fclose(out);
}

// 创建文件名 把文件后缀的.huf去掉即可
// 如果没有呢，就把现在的文件名后面加上.dehuf
// 把这个包装一下
string_t get_decompress_filename(char* filename) {
    string_t file = make_string("%s", filename);
    if (string_is_suffix(&file, ".huf")) {
        // 去掉此后缀
        string_popback(&file);
        string_popback(&file);
        string_popback(&file);
        string_popback(&file);
    } else {
        // 添加后缀
        string_append(&file, ".dehuf");
    }
    return file;
}

void huffman_decompress(char* filename) {
    // 解压缩分成几步
    // 1. 先读取file中的huffman树 并解析生成huffman node
    // 2. 然后依次读取bit流 遍历哈夫曼树生成字符序列 输出到文件中即可

    // 关键的问题是 我怎么知道huffman树的bit流结束了??
    FILE* in = open_file_for_read(filename);
    bitbuf_t* inbf = make_bitbuf(in);

    // generate huffman code tree from bit stream
    // 然后返回一个huffman code tree 的 root即可
    huffman_node_t* root = read_huffman_tree_from_file(inbf);

    // 不对 然后是读取字符的个数
    uint64_t len = 0;
    bitbuf_read_len(inbf, &len);

    // 打开被压缩文件
    string_t decompress_filename = get_decompress_filename(filename);
    FILE* out = open_file_for_write(decompress_filename.data);
    free_string(&decompress_filename);

    // 然后我们就可以根据这个root和接下来的bit流进行
    int bit = 0;
    huffman_node_t* work = root;
    for (uint64_t i = 0; i < len; i++) {
        // 每次读取一个byte
        work = root;
        while (true) {
            if (is_leaf(work)) {
                // 那么我们就输出叶子对应的字符就好
                fputc(work->c, out);
                break;
            }
            bitbuf_read_bit(inbf, &bit);
            if (bit == 0) {
                work = work->left;
            } else if (bit == 1) {
                work = work->right;
            } else {
                assert(false);
            }
        }
    }

    // 关闭文件和清理
    free_huffman_tree(root);
    free_bitbuf(inbf);
    fclose(in);
    fclose(out);
}
