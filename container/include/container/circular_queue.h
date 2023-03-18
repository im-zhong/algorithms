// 2023/3/18
// zhangzhong
// circular queue, ring buffer

#ifndef CONTAINER_CIRCULAR_QUEUE_H
#define CONTAINER_CIRCULAR_QUEUE_H

#include "container/strset.h"
#include "def.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    value_t* data;
    // size = capacity + 1
    // 表示实际分配的内存大小
    size_t size;
    size_t head;
    size_t tail;
    // 让capacity表示实际能存储的元素个数
    size_t capacity;
} circular_queue_t;

// 那用户当然不关心内部的实现细节
// 用户提供的参数应该就是它实际期望能够存储的元素个数
static circular_queue_t make_circular_queue(size_t capacity) {
    // 这样size > 0 就不会出现 除0错误
    assert(capacity > 0);
    circular_queue_t queue;
    queue.data = NULL;
    queue.capacity = capacity;
    queue.size = capacity + 1;
    queue.head = 0;
    queue.tail = 0;
    queue.data = malloc(sizeof(value_t) * queue.size);
    return queue;
}

static void free_circular_queue(circular_queue_t* queue) {
    if (queue) {
        free(queue->data);
        queue->data = NULL;
        queue->size = 0;
        queue->capacity = 0;
        queue->head = 0;
        queue->tail = 0;
    }
}

static bool circular_queue_empty(const circular_queue_t* queue) {
    assert(queue);
    // head == tail 时为空
    return queue->head == queue->tail;
}

static bool circular_queue_full(const circular_queue_t* queue) {
    assert(queue);
    // (tail + 1) % size == head 时为满
    // 想象 tail 还差一个格子就追上head了
    // size实际上只能存储size-1个元素 因为需要一个额外的元素帮助判断队列是否满了
    // 也就是tail指向的哪个格子是不用的
    return (queue->tail + 1) % queue->size == queue->head;
}

static void circular_queue_push(circular_queue_t* queue, value_t value) {
    assert(queue);
    // if (circular_queue_full(queue)) {
    //     return false;
    // }
    assert(!circular_queue_full(queue));
    // 如果队列中存在数据的话
    // head始终指向第一个数据 也就是top
    // 而tail始终指向将要push进去的数据
    queue->data[queue->tail] = value;
    // 其实循环队列需要注意的地方就是 指针移动的时候需要取模 这样才能成为一个环
    queue->tail = (queue->tail + 1) % queue->size;
    // return true;
}

static void circular_queue_pop(circular_queue_t* queue) {
    assert(queue);
    assert(!circular_queue_empty(queue));
    // 如果队列中存在数据的话
    // head始终指向第一个数据 也就是top
    // 而tail始终指向将要push进去的数据

    // value_t value = queue->data[queue->head];
    // 其实循环队列需要注意的地方就是 指针移动的时候需要取模 这样才能成为一个环
    queue->head = (queue->head + 1) % queue->size;
    // return value;
}

static value_t circular_queue_top(const circular_queue_t* queue) {
    assert(queue);
    assert(!circular_queue_empty(queue));
    return queue->data[queue->head];
}

#endif // CONTAINER_CIRCULAR_QUEUE_H
