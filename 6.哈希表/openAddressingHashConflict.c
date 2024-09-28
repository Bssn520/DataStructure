#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * 哈希冲突会导致查询结果错误，严重影响哈希表的可用性。
 * 为了解决该问题，每当遇到哈希冲突时，我们就进行哈希表扩容，直至冲突消失为止。
 * 此方法简单粗暴且有效，但效率太低，因为哈希表扩容需要进行大量的数据搬运与哈希值计算。
 * 为了提升效率，我们可以采用以下策略:
 * 1. 改良哈希表数据结构，使得哈希表可以在出现哈希冲突时正常工作。
 * 2. 仅在必要时，即当哈希冲突比较严重时，才执行扩容操作。
 * 
 */

/**
 * 结构改良方法:
 * 1. 链式地址
 * 在原始哈希表中，每个桶仅能存储一个键值对。
 * 链式地址（separate chaining）将单个元素转换为链表，
 * 将键值对作为链表节点，将所有发生冲突的键值对都存储在同一链表中。
 * 
 * 基于链式地址实现的哈希表的操作方法发生了以下变化:
 * 查询元素：输入 key ，经过哈希函数得到桶索引，即可访问链表头节点，然后遍历链表并对比 key 以查找目标键值对。
 * 添加元素：首先通过哈希函数访问链表头节点，然后将节点（键值对）添加到链表中。
 * 删除元素：根据哈希函数的结果访问链表头部，接着遍历链表以查找目标节点并将其删除。
 * 
 * 链式地址存在以下局限性:
 * 占用空间增大：链表包含节点指针，它相比数组更加耗费内存空间。
 * 查询效率降低：因为需要线性遍历链表来查找对应元素。
 * 
 * 2. 开放寻址
 * (1). 线性探测
 * 采用固定步长的线性搜索来进行探测，其操作方法与普通哈希表有所不同。
 * 插入元素：通过哈希函数计算桶索引，若发现桶内已有元素，则从冲突位置向后线性遍历（步长通常为 1），直至找到空桶，将元素插入其中。
 * 查找元素：若发现哈希冲突，则使用相同步长向后进行线性遍历，直到找到对应元素，返回 value 即可；
 * 如果遇到空桶，说明目标元素不在哈希表中，返回 None 。
 */

/* 键值对 int->string */
typedef struct 
{
    int key;
    char* val;
} Pair;

/* 开放寻址哈希表 */
typedef struct 
{
    int size;               // 有效键值对数量
    int capacity;           // 哈希表容量
    double loadThres;       // 触发扩容的负载因子阈值
    int extendRatio;        // 扩容倍数
    Pair** buckets;         // 桶数组
    Pair* TOMBSTONE;       // 删除标记
} HashMapOpenAddressing;

HashMapOpenAddressing* newHashMapOpenAddressing();
void delHashMapOpenAddressing(HashMapOpenAddressing *hmp);
int hashFunc(HashMapOpenAddressing *hmp, int key);
double loadFactor(HashMapOpenAddressing* hmp);
int findBucketHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key);
char* getValHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key);
void putHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key, char *val);
void delItemHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key);
void extendHashMapOpenAddressing(HashMapOpenAddressing *hashMap);
void printHashMapOpenAddressing(HashMapOpenAddressing *hashMap);


int main()
{
    // 创建哈希表
    HashMapOpenAddressing* hmp = newHashMapOpenAddressing();

    // 插入元素
    putHashMapOpenAddressing(hmp, 1, "One");
    putHashMapOpenAddressing(hmp, 2, "Two");
    putHashMapOpenAddressing(hmp, 3, "Three");
    putHashMapOpenAddressing(hmp, 4, "Four");
    
    // 打印哈希表
    printf("初始哈希表：\n");
    printHashMapOpenAddressing(hmp);
    
    // 查询元素
    printf("查找 key 为 2 的值: %s\n", getValHashMapOpenAddressing(hmp, 2));
    printf("查找 key 为 5 的值: %s\n", getValHashMapOpenAddressing(hmp, 5));  // 不存在的键

    // 测试负载因子，触发扩容
    putHashMapOpenAddressing(hmp, 5, "Five");
    putHashMapOpenAddressing(hmp, 6, "Six");
    
    printf("扩容后的哈希表：\n");
    printHashMapOpenAddressing(hmp);

    // 删除元素
    printf("删除 key 为 3 的元素。\n");
    delItemHashMapOpenAddressing(hmp, 3);

    // 打印删除后的哈希表
    printf("删除后的哈希表：\n");
    printHashMapOpenAddressing(hmp);

    // 释放哈希表
    delHashMapOpenAddressing(hmp);
    
    return 0;
}


/* 构造函数 */
HashMapOpenAddressing* newHashMapOpenAddressing()
{
    HashMapOpenAddressing* hmp = malloc(sizeof(HashMapOpenAddressing));
    hmp->size = 0;
    hmp->capacity = 4;
    hmp->loadThres = 2.0 / 3.0;
    hmp->extendRatio = 2;
    hmp->buckets = calloc(hmp->capacity, sizeof(Pair));
    hmp->TOMBSTONE = malloc(sizeof(Pair));
    hmp->TOMBSTONE->key = -1;
    hmp->TOMBSTONE->val = "-1";

    return hmp;
}

/* 解构函数 */
void delHashMapOpenAddressing(HashMapOpenAddressing *hmp)
{
    // 释放桶数组
    for (int i = 0; i < hmp->capacity; i++)
    {
        Pair* pair = hmp->buckets[i];
        if (pair != NULL && pair != hmp->TOMBSTONE)
        {
            free(pair->val);
            free(pair);
        }
    }
    // 释放哈希表结构体
    free(hmp->buckets);
    free(hmp->TOMBSTONE);
    free(hmp);
}

/* 哈希函数 */
int hashFunc(HashMapOpenAddressing *hmp, int key)
{
    return key % hmp->capacity;
}

/* 负载因子 */
double loadFactor(HashMapOpenAddressing* hmp)
{
    return (double)hmp->size / (double)hmp->capacity;
}

/* 搜索key对应的桶索引 */
int findBucketHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key)
{
    int index = hashFunc(hashMap, key);
    int firstTombstone = -1;
    // 线性探测，当遇到空桶时跳出
    while (hashMap->buckets[index] != NULL) {
        // 若遇到 key ，返回对应的桶索引
        if (hashMap->buckets[index]->key == key) {
            // 若之前遇到了删除标记，则将键值对移动至该索引处
            if (firstTombstone != -1) {
                hashMap->buckets[firstTombstone] = hashMap->buckets[index];
                hashMap->buckets[index] = hashMap->TOMBSTONE;
                return firstTombstone; // 返回移动后的桶索引
            }
            return index; // 返回桶索引
        }
        // 记录遇到的首个删除标记
        if (firstTombstone == -1 && hashMap->buckets[index] == hashMap->TOMBSTONE) {
            firstTombstone = index;
        }
        // 计算桶索引，越过尾部则返回头部
        index = (index + 1) % hashMap->capacity;
    }
    // 若 key 不存在，则返回添加点的索引
    return firstTombstone == -1 ? index : firstTombstone;
}

/* 查询操作 */
char* getValHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key)
{
    // 搜索 key 对应的桶索引
    int index = findBucketHashMapOpenAddressing(hashMap, key);
    // 若找到键值对，则返回对应 val
    if (hashMap->buckets[index] != NULL && hashMap->buckets[index] != hashMap->TOMBSTONE) {
        return hashMap->buckets[index]->val;
    }
    // 若键值对不存在，则返回空字符串
    return "";
}

/* 添加操作 */
void putHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key, char *val)
{
    // 当负载因子超过阈值时，执行扩容
    if (loadFactor(hashMap) > hashMap->loadThres) {
        extendHashMapOpenAddressing(hashMap);
    }
    // 搜索 key 对应的桶索引
    int index = findBucketHashMapOpenAddressing(hashMap, key);
    // 若找到键值对，则覆盖 val 并返回
    if (hashMap->buckets[index] != NULL && hashMap->buckets[index] != hashMap->TOMBSTONE) {
        free(hashMap->buckets[index]->val);
        hashMap->buckets[index]->val = (char *)malloc(sizeof(strlen(val) + 1));
        strcpy(hashMap->buckets[index]->val, val);
        hashMap->buckets[index]->val[strlen(val)] = '\0';
        return;
    }
    // 若键值对不存在，则添加该键值对
    Pair *pair = (Pair *)malloc(sizeof(Pair));
    pair->key = key;
    pair->val = (char *)malloc(sizeof(strlen(val) + 1));
    strcpy(pair->val, val);
    pair->val[strlen(val)] = '\0';

    hashMap->buckets[index] = pair;
    hashMap->size++;
}

/* 删除操作 */
void delItemHashMapOpenAddressing(HashMapOpenAddressing *hashMap, int key)
{
    // 搜索 key 对应的桶索引
    int index = findBucketHashMapOpenAddressing(hashMap, key);
    // 若找到键值对，则用删除标记覆盖它
    if (hashMap->buckets[index] != NULL && hashMap->buckets[index] != hashMap->TOMBSTONE) {
        Pair *pair = hashMap->buckets[index];
        free(pair->val);
        free(pair);
        hashMap->buckets[index] = hashMap->TOMBSTONE;
        hashMap->size--;
    }
}

/* 扩容哈希表 */
void extendHashMapOpenAddressing(HashMapOpenAddressing *hashMap)
{
    // 暂存原哈希表
    Pair **bucketsTmp = hashMap->buckets;
    int oldCapacity = hashMap->capacity;
    // 初始化扩容后的新哈希表
    hashMap->capacity *= hashMap->extendRatio;
    hashMap->buckets = (Pair **)calloc(hashMap->capacity, sizeof(Pair *));
    hashMap->size = 0;
    // 将键值对从原哈希表搬运至新哈希表
    for (int i = 0; i < oldCapacity; i++) {
        Pair *pair = bucketsTmp[i];
        if (pair != NULL && pair != hashMap->TOMBSTONE) {
            putHashMapOpenAddressing(hashMap, pair->key, pair->val);
            free(pair->val);
            free(pair);
        }
    }
    free(bucketsTmp);
}

/* 打印哈希表 */
void printHashMapOpenAddressing(HashMapOpenAddressing *hashMap)
{
    for (int i = 0; i < hashMap->capacity; i++) {
        Pair *pair = hashMap->buckets[i];
        if (pair == NULL) {
            printf("NULL\n");
        } else if (pair == hashMap->TOMBSTONE) {
            printf("TOMBSTONE\n");
        } else {
            printf("%d -> %s\n", pair->key, pair->val);
        }
    }
}