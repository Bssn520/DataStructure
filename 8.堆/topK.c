#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** 基于堆查找数组中最大的 k 个元素的函数 */
// 请注意：我们将堆中所有元素取反，从而用大顶堆来模拟小顶堆
int* topKHeap(int* nums, int sizeNum, int k)
{
    MaxHeap* maxHeap = newMaxHeap();

    // 前 k 个元素入堆
    for (size_t i = 0; i < k; i++)
        pushMaxHeap(maxHeap, nums[i]);
    
    // 从第 k+1 个元素开始，保持堆的长度为 k
    for (size_t i = k; i < sizeNum; i++)
    {
        // 若当前元素大于堆顶元素，则将堆顶元素出堆、当前元素入堆
        if (nums[i] > peekMinHeap(maxHeap))
        {
            popMinHeap(maxHeap);
            pushMinHeap(maxHeap, nums[i]);
        }
    }
    int* res = getMinHeap(maxHeap);
    // 释放内存
    destroyMaxHeap(maxHeap);
    
    return res;
}