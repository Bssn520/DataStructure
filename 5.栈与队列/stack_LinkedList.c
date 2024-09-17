#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * 栈（stack）是一种遵循先入后出逻辑的线性数据结构。
 * 
 * 我们把堆叠元素的顶部称为“栈顶”，底部称为“栈底”。
 * 将把元素添加到栈顶的操作叫作“入栈”，删除栈顶元素的操作叫作“出栈”。
 */

/**
 * 为了深入了解栈的运行机制，我们来尝试自己实现一个栈类。
 * 栈遵循先入后出的原则，因此我们只能在栈顶添加或删除元素。
 * 然而，数组和链表都可以在任意位置添加和删除元素，因此栈可以视为一种受限制的数组或链表。
 * 换句话说，我们可以“屏蔽”数组或链表的部分无关操作，使其对外表现的逻辑符合栈的特性。
 */

/**
 * 1. 基于链表实现栈
 * 使用链表实现栈时，我们可以将链表的头节点视为栈顶，尾节点视为栈底。
 * 对于入栈操作，我们只需将元素插入链表头部，这种节点插入方法被称为“头插法”。
 * 而对于出栈操作，只需将头节点从链表中删除即可。
 */

/* 定义一个链表 */
typedef struct ListNode
{
    int val;
    struct ListNode* next;
} ListNode;


/* 基于链表实现的栈 */
typedef struct 
{
    ListNode* top; // 将头节点作为栈顶
    int size; // 栈的长度
} LinkedListStack;

/* 构造函数 */
LinkedListStack* newLinkedListStack()
{
    // 分配内存
    LinkedListStack* s = malloc(sizeof(LinkedListStack));
    // 初始化
    s->top = NULL;
    s->size = 0;

    return s;
}

/* 析构函数 */
void destoryLinkedListStack(LinkedListStack* stack)
{
    while (stack->top)
    {
        // 拿到栈顶下面的那个节点
        ListNode* n = stack->top->next;
        // 释放掉栈顶元素
        free(stack->top);
        // 使头节点指向第二个节点
        stack->top = n;
    }
    // 释放栈
    free(stack);
}

/* 获取栈的长度 */
int size(LinkedListStack* stack)
{
    return stack->size;
}

/* 判断栈是否为空 */
bool isEmpty(LinkedListStack* stack)
{
    return stack->size == 0;
}

/* 入栈 */
void push(LinkedListStack* stack, int val)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    // 更新新加节点指针域
    node->next = stack->top;
    // 更新新加节点数据域
    node->val = val;
    // 更新栈顶
    stack->top = node;
    // 更新栈大小
    stack->size++;
}

/* 访问栈顶元素 */
int peek(LinkedListStack* stack)
{
    if (stack->size == 0)
    {
        printf("栈为空\n");
        return INT8_MAX;
    }

    return stack->top->val;
}

/* 出栈 */
int pop(LinkedListStack* stack)
{
    int val = peek(stack);

    ListNode* tmp = stack->top;
    stack->top = stack->top->next;

    // 释放内存
    free(tmp);
    stack->size--;

    return val;
}




int main()
{
    /**
     * 链表栈测试
     */

    // 获得栈
    LinkedListStack* stack = newLinkedListStack();

    // 添加元素
    push(stack, 1);
    push(stack, 3);
    push(stack, 2);
    push(stack, 5);
    push(stack, 4);
    printf("栈顶元素: %d\n", peek(stack)); // 4
    printf("栈长度: %d\n\n", size(stack)); // 5

    // 出栈
    pop(stack);
    printf("栈顶元素: %d\n", peek(stack)); // 5
    printf("栈长度: %d\n\n", size(stack)); // 4

    // 判断栈是否为空
    printf("栈是否为空: %d\n\n", isEmpty(stack)); // 0 表示false

    // 销毁栈
    destoryLinkedListStack(stack);

    return 0;
}