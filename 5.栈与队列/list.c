#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/* 构造函数 */
MyList* newMyList()
{
    MyList* list = (MyList*)malloc(sizeof(MyList));
    list->capacity = 10; // 列表容量默认初始化为10
    list->arr = malloc(sizeof(int) * list->capacity); // 为列表内部数组分配内存
    list->size = 0;
    list->extendRadio = 2;

    return list;
}


/* 析构函数 */
void destoryMyList(MyList* list)
{
    if (list != NULL) {
        free(list->arr);
        list->arr = NULL;
        free(list);
        list = NULL;
    }
}


/* 获取列表长度 */
int size(MyList* list)
{
    return list->size;   
}


/* 获取列表容量 */
int capacity(MyList* list)
{
    return list->capacity;
}


/* 扩容列表 */
void extendCapacity(MyList* list)
{
    // 先分配空间: 容量 * 扩容倍数
    int newCapacity = capacity(list) * list->extendRadio; // 计算目的扩容倍数
    int* extend = (int*)malloc(sizeof(int) * newCapacity); // 拿到扩容后的内存
    if (extend == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);  // 内存分配失败时，终止程序
    }
    int* temp = list->arr;

    // 拷贝旧数据到新数据
    for(int i = 0; i < size(list); i++)
        extend[i] = list->arr[i];

    // 释放旧数据
    free(temp);

    // 更新新数据
    list->arr = extend;
    list->capacity = newCapacity;
}


/* 访问元素 */
int getElement(MyList* list, int index)
{
    if (index >= 0 && index < list->size)
    {
        return list->arr[index];
    }
    
    return -1;
}


/* 更新元素 */
int setElement(MyList* list, int index, int val)
{
    if (index >= 0 && index < list->size)
    {
        list->arr[index] = val;
        return 0;
    }

    return -1;
}


/* 在列表尾部追加元素 */
void pushElement(MyList* list, int val)
{
    if (size(list) == capacity(list))
        extendCapacity(list); // 扩容
    
    list->arr[size(list)] = val;
    list->size++;
}


/* 在列表中插入元素 */
void insertElement(MyList* list, int index, int val)
{
    if (index < 0 || index > size(list))
        exit(1);
    
    if (size(list) == capacity(list))
        extendCapacity(list);
    
    for (int i = size(list); i > index; i--)
    {
        list->arr[i] = list->arr[i - 1]; // i - 1的索引正是最后一个元素的索引
    }
    list->arr[index] = val;
    list->size++;
}


/* 删除元素 */
int delElement(MyList* list, int index)
{
    if (index < 0 || index >= size(list))
        return -1;
    
    int num = list->arr[index];
    for (int i = index; i < size(list) - 1; i++)
    {
        list->arr[i] = list->arr[i + 1]; // 把index后的元素都向前移动一位
    }
    list->size--;
    
    return num;
}


/* 将列表转换为 Array 用于打印 */
int* toArray(MyList* list)
{
    return list->arr;
}

/* 遍历打印数组中的值 */
void arrPrint(int* arr, int size)
{
    printf("[");
    for (int i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i != size - 1)
            printf(", ");
    }
    printf("]\n");
}