#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 哈希表（hash table），又称散列表
 * 它通过建立键 key 与值 value 之间的映射，实现高效的元素查询。
 * 具体而言，我们向哈希表中输入一个键 key ，则可以在O(1)时间内获取对应的值 value
 */

/**
 * 我们先考虑最简单的情况，仅用一个数组来实现哈希表。
 * 在哈希表中，我们将数组中的每个空位称为桶（bucket），每个桶可存储一个键值对。
 * 
 * 哈希函数的计算过程分为以下两步:
 * 1. 通过某种哈希算法 hash() 计算得到哈希值。
 * 2. 将哈希值对桶数量（数组长度）capacity 取模，从而获取该 key 对应的数组索引 index 。
 * 
 */

#define MAX_SIZE 100

/* 键值对 int->string */
typedef struct 
{
    int key;
    char* val;
} Pair;

/* MapSet结构 */
typedef struct
{
    void *set;  // 指向键值对的数组
    int len;    // 键值对的数量
} MapSet;

/* 基于数组实现的哈希表 */
typedef struct 
{
    Pair* buckets[MAX_SIZE];
} ArrayHashMap;

/* 哈希函数 */
int hashFunc(int key)
{
    return key % MAX_SIZE;
}

/* 构造函数 */
ArrayHashMap* newArrayHashMap()
{
    // 分配内存
    ArrayHashMap* hmap = malloc(sizeof(ArrayHashMap));
    // 初始化
    for (int i = 0; i < MAX_SIZE; i++)
        hmap->buckets[i] = NULL;
    
    return hmap;
}

/* 析构函数 */
void destroyArrayHashMap(ArrayHashMap* hmap)
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
        {
            free(hmap->buckets[i]->val);
            free(hmap->buckets[i]);
        }
    }
    free(hmap);
}

/* 添加操作 */
void putArrayHashMap(ArrayHashMap* hmap, const int key, const char* val)
{
    // 分配内存
    Pair* Pair = malloc(sizeof(Pair));
    // 赋值
    Pair->key = key;
    Pair->val = malloc(strlen(val) + 1); // 为 val 分配内存，val 是存储值的字符串, +1 是为了存储字符串结尾的 \0
    strcpy(Pair->val, val); // 将传入的字符串复制到Pair结构体的val字段中去。
    // 通过 hashFunc 函数计算给定 key 的哈希值。
    int index = hashFunc(key);
    // 根据计算出来的 index，将刚刚创建的 Pair 放入 ArrayHashMap 的 buckets 数组中。
    hmap->buckets[index] = Pair;
}

/* 删除操作 */
void delItemArrayHashMap(ArrayHashMap* hmap, const int key)
{
    int index = hashFunc(key);
    free(hmap->buckets[index]->val);
    free(hmap->buckets[index]);
    hmap->buckets[index] = NULL;
}

/* 获取所有键值对 */
void pairSetArrayHashMap(ArrayHashMap* hmap, MapSet* set)
{
    // 声明所需对象
    Pair* entries;
    int i = 0, index = 0, total = 0;
    // 先统计哈希表中有效键值对数量
    for (i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
            total++;
    }
    // 复制有效键值对到 entries 数组
    entries = malloc(sizeof(Pair) * total);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
        {
            entries[index].key = hmap->buckets[i]->key;
            entries[index].val = malloc(strlen(hmap->buckets[i]->val) + 1);
            strcpy(entries[index].val, hmap->buckets[i]->val);
            index++;
        }
    }
    set->set = entries;
    set->len = total;
}

/* 获取所有键 */
void keySetArrayHashMap(ArrayHashMap* hmap, MapSet* set)
{
    int* keys;
    int i = 0, index = 0, total = 0;
    // 先统计哈希表中有效键值对数量
    for (i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
            total++;
    }

    keys = malloc(sizeof(int) * total);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
        {
            keys[index] = hmap->buckets[i]->key;
            index++;
        }
    }
    set->set = keys;
    set->len = total;
}

/* 获取所有值 */
void valueSetArrayHashMap(ArrayHashMap* hmap, MapSet* set)
{
    char** vals;
    int i = 0, index = 0, total = 0;
    // 先统计哈希表中有效键值对数量
    for (i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
            total++;
    }

    vals = malloc(sizeof(char*) * total);
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (hmap->buckets[i] != NULL)
        {
            vals[index] = hmap->buckets[i]->val;
            index++;
        }
    }
    set->set = vals;
    set->len = total;
}

/* 打印哈希表 */
void printArrayHashMap(ArrayHashMap* hmap)
{
    int i;
    MapSet set;
    pairSetArrayHashMap(hmap, &set);
    Pair* entries = (Pair*)set.set;
    for (int i = 0; i < set.len; i++)
    {
        printf("%d -> %s\n", entries[i].key, entries[i].val);
    }
    free(set.set);
}

int main() {
    // 创建哈希表
    ArrayHashMap* hmap = newArrayHashMap();

    // 向哈希表中添加一些键值对
    putArrayHashMap(hmap, 1, "One");
    putArrayHashMap(hmap, 2, "Two");
    putArrayHashMap(hmap, 3, "Three");
    putArrayHashMap(hmap, 4, "Four");
    
    printf("Initial HashMap:\n");
    printArrayHashMap(hmap);  // 打印初始哈希表内容

    // 删除一个键值对
    delItemArrayHashMap(hmap, 2);
    
    printf("\nHashMap after deletion of key 2:\n");
    printArrayHashMap(hmap);  // 打印删除后的哈希表内容

    // 获取并打印所有的键
    MapSet keySet;
    keySetArrayHashMap(hmap, &keySet);
    int* keys = (int*)keySet.set;
    printf("\nKeys in HashMap:\n");
    for (int i = 0; i < keySet.len; i++) {
        printf("%d\n", keys[i]);
    }
    free(keys);  // 释放存储键的内存

    // 获取并打印所有的值
    MapSet valueSet;
    valueSetArrayHashMap(hmap, &valueSet);
    char** vals = (char**)valueSet.set;
    printf("\nValues in HashMap:\n");
    for (int i = 0; i < valueSet.len; i++) {
        printf("%s\n", vals[i]);
    }
    free(valueSet.set);  // 释放存储值的内存

    // 销毁哈希表，释放内存
    destroyArrayHashMap(hmap);

    return 0;
}