#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * 建堆操作
 * 
 * 在某些情况下，我们希望使用一个列表的所有元素来构建一个堆，这个过程被称为“建堆操作”。
 * 
 * 通过遍历堆化实现:
 * 1. 将列表所有元素原封不动地添加到堆中，此时堆的性质尚未得到满足。
 * 2. 倒序遍历堆（层序遍历的倒序），依次对每个非叶节点执行“从顶至底堆化”。
 * 
 */

#define MAX_SIZE 10

/** 大顶堆的结构体 */
typedef struct 
{
    // 数组中实际含有的元素数量
    int size;
    // 使用预先分配内存的数组，避免扩容
    int data[MAX_SIZE];
} MaxHeap;

/** 从节点 i 开始，从顶至底堆化 */
void shiftDown(MaxHeap* hp, int i)
{
    while (true)
    {
        // 判断节点 i, l, r 中值最大的节点，记为 max
        // 获取节点i的子节点
        int l = left(hp, i);
        int r = right(hp, i);
        int max = i;
        if (l != INT8_MAX && hp->data[l] > hp->data[max])
            max = l;
        if (r != INT8_MAX && hp->data[r] > hp->data[max])
            max = r;
        // 若节点 i 最大或索引 l, r 越界，则无须继续堆化，跳出
        if (max == i)
            break;
        // 交换节点
        swap(hp, i, max);
        // 继续向下堆化
        i = max;
    }
}

/* 构造函数，根据切片建堆 */
MaxHeap *newMaxHeap(int nums[], int size) {
    // 所有元素入堆
    MaxHeap *maxHeap = (MaxHeap *)malloc(sizeof(MaxHeap));
    maxHeap->size = size;
    memcpy(maxHeap->data, nums, size * sizeof(int));
    for (int i = parent(maxHeap, size - 1); i >= 0; i--) {
        // 堆化除叶节点以外的其他所有节点
        siftDown(maxHeap, i);
    }
    return maxHeap;
}