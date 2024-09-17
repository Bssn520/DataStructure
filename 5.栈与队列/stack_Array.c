#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h" // 动态数组实现

// 编译命令: gcc stack_Array.c list.c -o stack_Array

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
 * 2. 基于列表实现栈
 * 使用数组实现栈时，我们可以将数组的尾部作为栈顶。
 * 入栈与出栈操作分别对应在数组尾部添加元素与删除元素，时间复杂度都为O(1)。
 * 由于入栈的元素可能会源源不断地增加，因此我们可以使用动态数组，这样就无须自行处理数组扩容问题。
 */

/* 基于数组实现的栈 */

typedef struct
{
    MyList* list;
    int size;
} ArrayStack;

/* 构造函数 */
ArrayStack* newArrayStack(MyList* list)
{
    // 分配内存
    ArrayStack* s = malloc(sizeof(ArrayStack));
    // 初始化栈
    s->list = list;
    s->size = size(s->list);

    return s;
}

/* 析构函数 */
void destoryArrayStack(ArrayStack* s)
{
    destoryMyList(s->list);

    free(s);
}

/* 判断栈是否为空 */
bool isEmptyArrayStack(ArrayStack* s)
{
    return s->size == 0;
}

/* 入栈 */
void pushArrayStack(ArrayStack* s, int val)
{
    // 采用了动态数组，故不用考虑容量问题
    pushElement(s->list, val); // 使用list.c中的函数, 以便其自动扩容
    s->size = size(s->list);
}

/* 访问栈顶元素 */
int peekArrayStack(ArrayStack* s)
{
    if (isEmptyArrayStack(s))
    {
        printf("栈为空\n");
        return INT8_MAX;
    }
    
    int val = getElement(s->list, s->size - 1);

    return val;
}

/* 出栈 */
int popArrayStack(ArrayStack* s)
{
    if (isEmptyArrayStack(s))
    {
        printf("栈为空\n");
        return INT8_MAX;
    }

    int val = delElement(s->list, size(s->list) - 1);
    s->size = size(s->list);

    return val;
}



int main(void)
{
    MyList* list = newMyList();
    ArrayStack* stack = newArrayStack(list);

    // 添加元素
    for(int i = 0; i < 100; i++)
    {
        pushArrayStack(stack, i);
    }
    printf("栈顶元素: %d\n", peekArrayStack(stack)); // 99
    printf("栈长度: %d\n\n", stack->size); // 100

    // 出栈
    popArrayStack(stack);
    printf("栈顶元素: %d\n", peekArrayStack(stack)); // 98
    printf("栈长度: %d\n\n", stack->size); // 99

    // 判断栈是否为空
    printf("栈是否为空: %d\n\n", isEmptyArrayStack(stack)); // 0 表示false

    // 销毁栈
    destoryArrayStack(stack);

    return 0;
}
