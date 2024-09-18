#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * 队列（queue）是一种遵循先入先出规则的线性数据结构。
 * 
 * 顾名思义，队列模拟了排队现象，即新来的人不断加入队列尾部，而位于队列头部的人逐个离开。
 * 将队列头部称为“队首”，尾部称为“队尾”，将把元素加入队尾的操作称为“入队”，删除队首元素的操作称为“出队”。
 */

/**
 * 1. 基于链表实现的队列
 * 
 * 将链表的“头节点”和“尾节点”分别视为“队首”和“队尾”，规定队尾仅可添加节点，队首仅可删除节点。
 */

/* 链表节点 */
typedef struct ListNode
{
    int val;
    struct ListNode* next;
} ListNode;

/* 链表节点构造函数 */
ListNode* newListNode(int val)
{
    // 分配内存
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    // 初始化节点
    node->next = NULL;
    node->val = val;

    return node;
}

/* 基于链表实现的队列 */
typedef struct 
{
    ListNode* front, *rear;
    int queSize;
} LinkedListQueue;

/* 队列构造函数 */
LinkedListQueue* newLinkedListQueue()
{
    // 分配内存
    LinkedListQueue* queue = malloc(sizeof(LinkedListQueue));
    // 初始化
    queue->front = NULL;
    queue->rear = NULL;
    queue->queSize = 0;

    return queue;
}

/* 析构函数 */
void destroyLinkedListQueue(LinkedListQueue* q)
{
    // 释放所有节点
    while (q->front != NULL)
    {
        // 改变节点指向，然后释放节点
        ListNode* tmp = q->front;
        q->front = q->front->next;
        free(tmp);
    }
    // 释放queue结构体
    free(q);
}

/* 获取队列长度 */
int sizeLinkedListQueue(LinkedListQueue* q)
{
    return q->queSize;
}

/* 判断队列是否为空 */
bool isEmptyLinkedListQueue(LinkedListQueue* q)
{
    return (sizeLinkedListQueue(q) == 0);
}

/* 入队 */
void pushLinkedListQueue(LinkedListQueue* q, int val)
{
    // 尾节点处添加 node
    ListNode* node = newListNode(val);
    // 如果队列为空，则令头、尾节点都指向该节点
    if (sizeLinkedListQueue(q) == 0)
    {
        q->front = node;
        q->rear = node;
    }
    // 如果队列不为空，则将该节点添加到尾节点后
    else
    {
        q->rear->next = node;
        q->rear = node;
    }
    q->queSize++;
}

/* 访问队首元素 */
int peekLinkedListQueue(LinkedListQueue* q)
{
    if (sizeLinkedListQueue(q) == 0)
    {
        printf("队列为空\n");
        return INT8_MAX;
    }

    return q->front->val;
}

/* 出队 */
int popLinkedListQueue(LinkedListQueue* q)
{
    if (q->queSize == 0)
    {
        printf("队列为空\n");
        return INT8_MAX;
    }

    int val = peekLinkedListQueue(q);

    // 改变头节点指向第二个节点
    ListNode* tmp = q->front;
    q->front = q->front->next;
    free(tmp);
    q->queSize--;

    return val;
}

/* 打印队列 */
void printLinkedListQueue(LinkedListQueue* q)
{
    printf("[");
    ListNode* node = q->front;
    while (node != NULL)
    {
        printf("%d", node->val);
        if (node->next != NULL)
            printf(", ");
        node = node->next;
    }
    printf("]\n");
}

int main()
{
    /**
     * 链表队列测试 
     */
    LinkedListQueue* queue = newLinkedListQueue();

    /* 判断队列是否为空 */
    printf("队列是否为空(0假1真): %d\n\n", isEmptyLinkedListQueue(queue)); // 1

    /* 入队测试 */
    for (int i = 0; i < 5; i++)
    {
        pushLinkedListQueue(queue, i);
    }
    printLinkedListQueue(queue); // [0, 1, 2, 3, 4]

    /* 访问队首元素 */
    printf("队首元素: %d\n\n", peekLinkedListQueue(queue)); // 队首元素: 0

    /* 出队测试 */
    popLinkedListQueue(queue);
    printLinkedListQueue(queue); // [1, 2, 3, 4]

    /* 获取队列长度 */
    printf("队列长度: %d\n\n", sizeLinkedListQueue(queue)); // 队列长度: 4

    /* 判断队列是否为空 */
    printf("队列是否为空(0假1真): %d\n\n", isEmptyLinkedListQueue(queue)); // 队列是否为空(0假1真): 0

    /* 释放 */
    destroyLinkedListQueue(queue);

    return 0;
}