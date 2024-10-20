#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 15

/**
 * 堆 
 * 
 * 堆（heap）是一种满足特定条件的完全二叉树，主要可分为两种类型。
 * 小顶堆（min heap）：任意节点的值 <= 其子节点的值。
 * 大顶堆（max heap）：任意节点的值 >= 其子节点的值。
 * 
 * 堆作为完全二叉树的一个特例，具有以下特性:
 * 1. 最底层节点靠左填充，其他层的节点都被填满。
 * 2. 我们将二叉树的根节点称为“堆顶”，将底层最靠右的节点称为“堆底”。
 * 3. 对于大顶堆（小顶堆），堆顶元素（根节点）的值是最大（最小）的。
 * 
 * 堆通常用于实现优先队列，大顶堆相当于元素按从大到小的顺序出队的优先队列。从使用角度来看，我们可以将“优先队列”和“堆”看作等价的数据结构。
 * 
 */

/** 大顶堆的结构体 */
typedef struct 
{
    // 数组中实际含有的元素数量
    int size;
    // 使用预先分配内存的数组，避免扩容
    int data[MAX_SIZE];
} MaxHeap;

/** 函数声明 */
/** 构造函数 */
MaxHeap* newMaxHeap();
/** 析构函数 */
void destroyMaxHeap(MaxHeap* heap);
/** 获取左子节点索引 */
int left(MaxHeap* heap, int i);
/** 获取右子节点索引 */
int right(MaxHeap* heap, int i);
/** 获取父节点索引 */
int parent(MaxHeap* heap, int i);
/** 交换两个节点的值 */
void swap(MaxHeap* heap, int i, int j);
/** 获取堆大小 */
int sizeMaxHeap(MaxHeap* hp);
/** 判断堆是否为空 */
bool isEmpty(MaxHeap* hp);
/** 访问堆顶元素 */
int peekMaxHeap(MaxHeap* hp);
/** 从节点 i 开始，从顶至底堆化 */
void shiftDown(MaxHeap* hp, int i);
/** 从节点 i 开始，从底至顶堆化 */
void shiftUp(MaxHeap* hp, int i);
/** 元素入堆 */
void pushMaxHeap(MaxHeap* hp, int val);
/** 元素出堆 */
int popMaxHeap(MaxHeap* hp);


int main() {
    // 创建一个大顶堆
    MaxHeap* heap = newMaxHeap();

    if (heap == NULL) {
        printf("堆创建失败\n");
        return 1;
    }

    // 插入元素到堆中
    pushMaxHeap(heap, 10);
    pushMaxHeap(heap, 20);
    pushMaxHeap(heap, 5);
    pushMaxHeap(heap, 8);
    pushMaxHeap(heap, 15);
    pushMaxHeap(heap, 30);
    
    printf("当前堆大小: %d\n", sizeMaxHeap(heap));

    // 打印堆中元素（从堆顶到堆底）
    printf("当前堆元素: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");

    // 访问堆顶元素
    printf("堆顶元素: %d\n", peekMaxHeap(heap));

    // 弹出堆顶元素
    int poppedVal = popMaxHeap(heap);
    printf("弹出堆顶元素: %d\n", poppedVal);

    // 打印弹出后的堆元素
    printf("弹出后堆元素: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");

    // 销毁堆
    destroyMaxHeap(heap);

    return 0;
}


/** 构造函数 */
MaxHeap* newMaxHeap()
{
    // 分配内存
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    if (heap == NULL)
    {
        printf("MaxHeap malloc failed");
        return NULL;
    }
    // 初始化
    heap->size = 0;
    for (size_t i = 0; i < MAX_SIZE; i++)
        heap->data[i] = 0;
    

    return heap;
}

/** 析构函数 */
void destroyMaxHeap(MaxHeap* heap)
{
    free(heap);
}

/** 获取左子节点索引 */
int left(MaxHeap* heap, int i)
{
    int index = i * 2 + 1;
    if (index >= heap->size)
        return INT8_MAX;

    return index;
}

/** 获取右子节点索引 */
int right(MaxHeap* heap, int i)
{
    int index = i * 2 + 2;
    if (index >= heap->size)
        return INT8_MAX;

    return index;
}

/** 获取父节点索引 */
int parent(MaxHeap* heap, int i)
{
    int index = (i - 1) / 2;
    if (i == 0)
        return INT8_MAX;

    return index;
}

/** 交换两个节点的值 */
void swap(MaxHeap* heap, int i, int j)
{
    int tmp = heap->data[i];
    heap->data[i] = heap->data[j];
    heap->data[j] = tmp;
}

/** 获取堆大小 */
int sizeMaxHeap(MaxHeap* hp)
{
    return hp->size;
}

/** 判断堆是否为空 */
bool isEmpty(MaxHeap* hp)
{
    return hp->size == 0;
}

/** 访问堆顶元素 */
int peekMaxHeap(MaxHeap* hp)
{
    return hp->data[0];
}

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

/** 从节点 i 开始，从底至顶堆化 */
void shiftUp(MaxHeap* hp, int i)
{
    while (true)
    {
        // 获取节点i的父节点
        int p = parent(hp, i);
        // 当越过根节点 或 节点值小于父节点值时跳出
        if (p == INT8_MAX || hp->data[i] <= hp->data[p])
            break;
        // 交换节点
        swap(hp, i, p);
        // 继续向上堆化
        i = p;
    }
}

/** 元素入堆 */
void pushMaxHeap(MaxHeap* hp, int val)
{
    // 判断堆是否满了
    if (hp->size == MAX_SIZE)
    {
        printf("堆满");
        return;
    }
    
    hp->data[hp->size] = val;
    hp->size++;

    // 从底至顶堆化
    shiftUp(hp, hp->size - 1);
}

/** 元素出堆 */
int popMaxHeap(MaxHeap* hp)
{
    // 判空处理
    if (isEmpty(hp))
    {
        printf("空堆");
        return INT8_MAX;
    }

    // 交换根节点与最右叶节点(交换首元素与尾元素)
    swap(hp, 0, hp->size - 1);
    // 删除节点
    int val = hp->data[hp->size - 1];
    hp->size--;
    // 从顶至底堆化
    shiftDown(hp, 0);

    return val;
}