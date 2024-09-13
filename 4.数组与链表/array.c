#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * 总的来看，数组的插入与删除操作有以下缺点:
 * 1. 时间复杂度高：数组的插入和删除的平均时间复杂度均为 O(n), 其中 n 为数组长度。
 * 2. 丢失元素：由于数组的长度不可变，因此在插入元素后，超出数组长度范围的元素会丢失。
 * 3. 内存浪费：我们可以初始化一个比较长的数组，只用前面一部分，
 *    这样在插入数据时，丢失的末尾元素都是“无意义”的，但这样做会造成部分内存空间浪费。
 */

/**
 * 随机生成一个 [0, szie) 之间的数作为数组索引
 * 返回把索引对应的数组元素
 */
int randomAccess(int* arr, int size)
{
    // get a random number from [0, size)
    int index = rand() % size;
    printf("Random index: %d\n", index);
    int val = arr[index];

    return val;
}


/**
 * 在数组的索引 index 处插入元素 num
 * 
 * 如果想在数组中间插入一个元素，
 * 则需要将该元素之后的所有元素都向后移动一位，之后再把元素赋值给该索引。
 */
void arrInsert(int* arr, int size, int num, int index)
{
    // 把 index 后的元素全部向后移一位
    for (int i = size - 1; i > index; i--)
    {
        // 把前一个元素的值赋值给后一个元素
        arr[i] = arr[i - 1];
    }
    // // 将 num 赋给 index 处的元素
    arr[index] = num;
}


/**
 * 删除元素
 * 
 * 若想删除索引 i 处的元素，则需要把索引 i 之后的元素都向前移动一位。
 */
void arrDelete(int* arr, int size, int index)
{
    // 把索引 index 之后的所有元素向前移动一位
    for (int i = index; i < size - 1; i++)
    {
        arr[i] = arr[i + 1];
    }
}


/**
 * 遍历数组
 */
int traverse(int* arr, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum;
}


/**
 * 在数组中查找元素
 * 
 * 在数组中查找指定元素需要遍历数组，每轮判断元素值是否匹配，若匹配则输出对应索引。
 * 因为数组是线性数据结构，所以上述查找操作被称为“线性查找”。
 */
int arrFind(int* arr, int size, int target)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == target)
            return i;
    }

    return -1;
}


/**
 * 扩容数组
 * 如果我们希望扩容数组，则需重新建立一个更大的数组，然后把原数组元素依次复制到新数组。
 * 在数组很大时，这种操作非常耗时。
 */
int* arrExtend(int* arr, int size, int enlarge)
{
    // 初始化扩容后的数组
    int* res = (int*)malloc(sizeof(int) * (size + enlarge));
    // 将原数组中的内容逐个复制到新数组中
    for (int i = 0; i < size; i++)
    {
        res[i] = arr[i];
    }
    // 初始化扩容后的空间
    for (int i = size; i < size + enlarge; i++)
    {
        res[i] = 0;
    }

    // 返回拓展后的新数组
    return res;
}

/**
 * 遍历打印数组中的值
 */
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

int main()
{
    // 初始化随机数生成器
    srand(time(0));

    int arr[5] = {1, 3, 5, 7, 9};

    /**
     *随机索引访问数组示例
     */
    int val = randomAccess(arr, 5);
    printf("Random value of array: %d\n\n", val);

    /**
     * 元素插入示例
     * 这种情况下，数组的最后一个元素将被丢弃
     */
    int arr2[5] = {1, 3, 5, 7, 9};
    printf("Before insert: %d\n", arr2[1]); // 3
    int num = 2;
    arrInsert(arr2, 5, num, 1);
    printf("After insert: %d\n\n", arr2[1]); // 2

    /**
     * 元素删除示例
     * 删除元素完成后，原先末尾的元素变得“无意义”了，所以我们无须特意去修改它。
     */
    int arr3[5] = {1, 3, 5, 7, 9};
    printf("Before delete: %d\n", arr3[1]); // 3
    arrDelete(arr3, 5, 1);
    printf("After delete: %d\n\n", arr3[1]); // 5

    /**
     * 遍历数组示例
     * 返回数组中元素和
     */
    int arr4[5] = {1, 3, 5, 7, 9};
    int sum = traverse(arr4, 5);
    printf("Sum: %d\n\n", sum);

    /**
     * 查找元素示例
     * 返回查找到的元素在数组中的索引
     */
    int arr5[5] = {1, 3, 5, 7, 9};
    int target = 7;
    int indexOfTarget = arrFind(arr5, 5, target);
    printf("Index of %d: %d\n\n", target, indexOfTarget);

    /**
     * 数组扩容示例
     * 先扩容然后在数组中插入元素，观察与没有扩容时插入元素的区别
     */
    int arr6[5] = {1, 3, 5, 7, 9};
    arrPrint(arr, 5); // 打印扩容前的数组

    // 对数组扩容
    int enlarge = 1;
    int* arr6_extened = arrExtend(arr, 5, enlarge);

    // 在数组中插入数值
    arrInsert(arr6_extened, (5 + enlarge), 2, 1);
    arrPrint(arr6_extened, 6); // 打印扩容后的数组

    // 释放分配的内存
    free(arr6_extened);

    return 0;
}