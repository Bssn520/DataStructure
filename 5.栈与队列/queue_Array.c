#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

/* 编译命令: gcc queue_Array.c list.c -o queue_Array */

/**
 * 队列（queue）是一种遵循先入先出规则的线性数据结构。
 * 
 * 顾名思义，队列模拟了排队现象，即新来的人不断加入队列尾部，而位于队列头部的人逐个离开。
 * 将队列头部称为“队首”，尾部称为“队尾”，将把元素加入队尾的操作称为“入队”，删除队首元素的操作称为“出队”。
 */

/**
 * 环形数组的知识
 * front: 指向队首元素
 * rear: 指向 队尾元素+1 的位置
 * queSize: 队列中有效元素的数量
 * 
 * 入队:
 * 入队即在rear处赋值
 * 1. 判断队列是否满了，(list->size == list->capacity) 或 ((rear + 1) % queCapacity == front)
 * 2. 如果未满, 计算 rear 的值，
 * rear = (front + queSize) % queCapacity;
 * 3. 赋值
 * list->arr[rear] = value;
 * 4. 递增 queSize 和 list->size
 * queSize++, list->size++;
 * 
 * 出队:
 * 即把front向后移动一位, 若越过队尾，则返回队列头部
 * 1. 计算front的值并赋值
 * newFront = (front + 1) % queCapacity; // 此处的queCapacity使用的是MyList的属性capacity
 * 2. 递减队列大小
 * queSize--;
 * 
 */

/**
 * 2. 基于数组实现队列
 * 
 * 在数组中删除首元素的时间复杂度为O(n),
 * 这会导致出队操作效率较低。然而，我们可以采用以下巧妙方法来避免这个问题:
 * 我们可以使用一个变量 front 指向队首元素的索引，并维护一个变量 size 用于记录队列长度。
 * 定义 rear = front + size ，这个公式计算出的 rear 指向队尾元素之后的下一个位置。
 * 数组中包含元素的有效区间为 [front, rear - 1],
 * 入队操作：将输入元素赋值给 rear 索引处，并将 size 增加 1 。
 * 出队操作：只需将 front 增加 1 ，并将 size 减少 1 。
 * 这样以来, 负责度为O(1)
 * 
 */

/* 基于环形数组实现的队列 */
typedef struct ArrayQueue
{
    MyList* list;
    int front; // 队首指针, 指向队首元素的索引
    int rear;  // 尾指针, 指向队尾元素之后的下一个位置
    int queSize; // 记录队列长度
} ArrayQueue;

/* 构造函数 */
ArrayQueue* newArrayQueue(MyList* list)
{
    // 分配内存
    ArrayQueue* q = malloc(sizeof(ArrayQueue));
    // 初始化
    q->list = list;
    q->front = 0;
    q->rear = 0;
    q->queSize = 0;

    return q;
}

/* 析构函数 */
void destroyArrayQueue(ArrayQueue* q)
{
    destoryMyList(q->list);
    free(q);
}

/* 获取队列长度 */
int sizeArrayQueue(ArrayQueue* q)
{
    return q->queSize;
}

/* 判断队列是否为空 */
bool isEmptyArrayQueue(ArrayQueue* q)
{
    return (q->queSize == 0);
}

/* 访问队首元素 */
int peekArrayQueue(ArrayQueue* q)
{
    if (q->queSize == 0)
    {
        printf("队列为空\n");
        return false;
    }

    return q->list->arr[q->front];
}

/* 入队 */
void pushArrayQueue(ArrayQueue* q, int val)
{
    if (q->list->size == q->list->capacity) // 有效元素数量 == 队列容量
    {
        extendCapacity(q->list);
    }
    // 计算队尾指针，指向队尾索引 + 1
    // 通过取余操作实现 rear 越过数组尾部后回到头部
    int rear = (q->front + q->queSize) % q->list->capacity;
    q->list->arr[rear] = val;
    q->queSize++, q->list->size++;
}

/* 出队 */
int popArrayQueue(ArrayQueue* q)
{
    if (q->queSize == 0)
    {
        printf("队列为空\n");
        return false;
    }

    int val = peekArrayQueue(q);
    // 队首指针向后移1位, 若越过队尾, 则返回队列头部
    q->front = (q->front + 1) % q->list->capacity;
    q->queSize--;

    return val;
}

/* 打印队列 */
void printArrayQueue(ArrayQueue* q)
{
    printf("[");
    for (int i = 0; i < q->queSize; i++)
    {
        // 使用环形数组方式从队首开始遍历队列中的有效元素
        int index = (q->front + i) % q->list->capacity;
        printf("%d", q->list->arr[index]);
        if (i != q->queSize - 1)
            printf(", ");
    }
    printf("]\n");
}

int main()
{
    /* 拿到动态数组 */
    MyList* list = newMyList();

    /* 拿到队列 */
    ArrayQueue* queue = newArrayQueue(list);

    /* 判断队列是否为空 */
    printf("队列是否为空(0假1真): %d\n\n", isEmptyArrayQueue(queue)); // 1

    /* 入队测试 */
    for (int i = 0; i < 100; i++)
    {
        pushArrayQueue(queue, i);
    }
    printArrayQueue(queue); // [0, 1, 2, 3, 4]

    /* 访问队首元素 */
    printf("队首元素: %d\n", peekArrayQueue(queue)); // 队首元素: 0
    printf("队列长度: %d\n\n", sizeArrayQueue(queue)); // 5

    /* 出队测试 */
    popArrayQueue(queue);
    printArrayQueue(queue); // [1, 2, 3, 4]
    printf("队首元素: %d\n", peekArrayQueue(queue)); // 队首元素: 1
    printf("队列长度: %d\n\n", sizeArrayQueue(queue)); // 4

    /* 判断队列是否为空 */
    printf("队列是否为空(0假1真): %d\n\n", isEmptyArrayQueue(queue)); // 队列是否为空(0假1真): 0

    /* 释放 */
    destroyArrayQueue(queue);

    return 0;
}