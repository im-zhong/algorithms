// 2022/12/9
// zhangzhong
// huffman code

#include "string/huffman.h"
#include "container/bheap.h"
#include "util/c_string.h"
#include <stdio.h>
#include <stdlib.h>

// 我们可以把打开文件 和 向文件中写入内容都放在一起
// 这样应用程序部分写起来就非常简单了

// 用 rewind 重置文件offset
#define ASCII_SIZE 256

huffman_node_t *make_huffman_node(int c, int freq) {
  huffman_node_t *node = (huffman_node_t *)malloc(sizeof(huffman_node_t));
  node->left = NULL;
  node->right = NULL;
  node->freq = freq;
  node->c = c;
  return node;
}

bool huffman_cmp(const void *n1, const void *n2) {
  return ((huffman_node_t *)n1)->freq < ((huffman_node_t *)n2)->freq;
}

static bool is_leaf(huffman_node_t *node) {
  return node->left == NULL && node->right == NULL;
}

// 不对不对 我们这里应该输出一个二进制bit
// 而不是一个字符 0 或者 1啊
// 这需要额外的数据结构支持
// 不行啊  我们不能直接输出一个bit呀 至少都是一个byte
// ... 怎么办
// todo: 写一个bitbuf
void write_huffman(FILE *out, huffman_node_t *root, string_t *code) {
  // 前序遍历
  if (root) {
    // 如果是leaf, 那么输出我们自己的字符串
    if (is_leaf(root)) {
      fprintf(out, "%s", code->data);
    } else {
      // 我们可能有左孩子或者右孩子
      // 先遍历左子树
      // string_pushback(code, 'char c')
    }
  }
}

void huffman_compress(FILE *stream) {
  // 第一遍读取整个字符流 直到

  // 还不行 因为不是所有的节点都会有频率 不能全部放到index_heap里面
  int freq[ASCII_SIZE];
  for (int i = 0; i < ASCII_SIZE; i++) {
    freq[i] = 0;
  }
  int c = 0;
  while ((c = fgetc(stream)) != EOF) {
    // 统计频率 那么请问 这个频率我记录在哪??
    // 我可以先记录在一个数组里面
    freq[c]++;
  }

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

  bheap_t heap;
  bheap_init(&heap, ASCII_SIZE, huffman_cmp);

  // 然后再次遍历freq 适当的填充leaf数组
  int i = 0;
  for (int c = 0; c < ASCII_SIZE; c++) {
    if (freq[c] != 0) {
      huffman_node_t *node = make_huffman_node(c, freq[c]);
      // 然后将此node 插入bheap
      bheap_push(&heap, node);
    }
  }

  // 然后创建index_heap
  // 不对 huffman node 会动态的从heap中删除 然后添加进去
  // index_heap不具备这个特性
  // 不能用index heap 就得用普通的bheap

  //

  // 然后我们需要构建哈夫曼树
  // 只要heap非空 就一直进行这个操作
  while (bheap_size(&heap) > 1) {

    // 当heap中只有一个节点的时候就返回

    // 弹出两个最小的节点
    // todo: 让pop同时返回存储元素的指针
    huffman_node_t *n1 = (huffman_node_t *)bheap_pop(&heap);
    huffman_node_t *n2 = (huffman_node_t *)bheap_pop(&heap);

    // 然后创建一一个新的节点
    huffman_node_t *node = make_huffman_node(0, n1->freq + n2->freq);
    node->left = n1;
    node->right = n2;
    bheap_push(&heap, node);
  }

  // bheap里面至少有一个元素
  assert(bheap_size(&heap) == 1);
  huffman_node_t *root = bheap_pop(&heap);

  // 这样我们就拿到了哈夫曼树的根
  // 然后我们需要前序遍历哈夫曼树 先将我们自己编码到输出文件中

  // 重置文件offset
  rewind(stream);
}