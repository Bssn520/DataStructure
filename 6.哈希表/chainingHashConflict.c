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

/* 链表节点 */
typedef struct Node
{
    Pair* pair;
    struct Node* next;
} Node;

/* 链式地址哈希表 */
typedef struct {
    int size;           // 键值对数量
    int capacity;       // 哈希表容量
    double loadThres;   // 触发扩容的负载因子阈值
    int extendRatio;    // 扩容倍数
    Node** buckets;     // 桶列表
} HashMapChaining;

HashMapChaining* newHashMapChaining();
void destroyHashMapChaining(HashMapChaining* hmp);
int hashFunc(HashMapChaining* hmp, int key);
double loadFactor(HashMapChaining* hmp);
char* getValHashMapChaining(HashMapChaining* hmp, int key);
void extendHashMapChaining(HashMapChaining* hmp);
void putHashMapChaining(HashMapChaining* hmp, int key, const char* val);
void delItemHashMapChaining(HashMapChaining *hmp, int key);
void printHashMapChaining(HashMapChaining* hmp);


int main() {
    // 创建哈希表
    HashMapChaining* hmp = newHashMapChaining();

    // 插入元素
    putHashMapChaining(hmp, 1, "One");
    putHashMapChaining(hmp, 2, "Two");
    putHashMapChaining(hmp, 3, "Three");
    putHashMapChaining(hmp, 4, "Four");
    
    // 打印哈希表
    printf("初始哈希表：\n");
    printHashMapChaining(hmp);
    
    // 查询元素
    printf("查找 key 为 2 的值: %s\n", getValHashMapChaining(hmp, 2));
    printf("查找 key 为 5 的值: %s\n", getValHashMapChaining(hmp, 5));  // 不存在的键

    // 测试负载因子，触发扩容
    putHashMapChaining(hmp, 5, "Five");
    putHashMapChaining(hmp, 6, "Six");
    
    printf("扩容后的哈希表：\n");
    printHashMapChaining(hmp);

    // 删除元素
    printf("删除 key 为 3 的元素。\n");
    delItemHashMapChaining(hmp, 3);

    // 打印删除后的哈希表
    printf("删除后的哈希表：\n");
    printHashMapChaining(hmp);

    // 释放哈希表
    destroyHashMapChaining(hmp);
    
    return 0;
}


/* 构造函数 */
HashMapChaining* newHashMapChaining()
{
    // 分配内存
    HashMapChaining* hmp = malloc(sizeof(HashMapChaining));
    // 初始化
    hmp->size = 0;
    hmp->capacity = 4;
    hmp->loadThres = 2.0 / 3.0;
    hmp->extendRatio = 2;
    hmp->buckets = malloc(sizeof(Node*) * hmp->capacity);
    for (int i = 0; i < hmp->capacity; i++)
    {
        hmp->buckets[i] = NULL;
    }
    
    return hmp;
}

/* 析构函数 */
void destroyHashMapChaining(HashMapChaining* hmp)
{
    for (int i = 0; i < hmp->capacity; i++)
    {
        // 释放桶
        Node* curr = hmp->buckets[i];
        // 释放桶内链表
        while (curr)
        {
            Node* tmp = curr;
            curr = curr->next;
            free(tmp->pair);
            free(tmp);
        }
    }
    free(hmp->buckets);
    free(hmp);
}

/* 哈希函数 */
int hashFunc(HashMapChaining* hmp, int key)
{
    return key % hmp->capacity;
}

/* 负载因子 */
double loadFactor(HashMapChaining* hmp)
{
    return (double)hmp->size / (double)hmp->capacity;
}

/* 查询操作 */
char* getValHashMapChaining(HashMapChaining* hmp, int key)
{
    int index = hashFunc(hmp, key);
    // 遍历桶
    Node* curr = hmp->buckets[index]; // 找到目标桶
    // 遍历目标桶内链表, 若找到 key, 则返回对应 val
    while (curr)
    {
        if (curr->pair->key == index)
        {
            return curr->pair->val;
        }
        curr = curr->next;
    }

    // 若未找到 key ，则返回空字符串
    return "";
}

/* 扩容哈希表 */
void extendHashMapChaining(HashMapChaining* hmp)
{
    // 暂存旧哈希表
    int oldCapacity = hmp->capacity;
    Node** oldBuckets = hmp->buckets;

    // 初始化扩容后的新哈希表
    hmp->capacity *= hmp->extendRatio;
    hmp->buckets = malloc(sizeof(Node*) * hmp->capacity);
    for (int i = 0; i < hmp->capacity; i++)
    {
        hmp->buckets[i] = NULL;
    }
    hmp->size = 0;

    // 将键值对从旧哈希表复制到新哈希表
    for (int i = 0; i < oldCapacity; i++)
    {
        Node* curr = oldBuckets[i];
        while (curr)
        {
            putHashMapChaining(hmp, curr->pair->key, curr->pair->val);
            Node* tmp = curr;
            curr = curr->next;
            free(tmp->pair);
            free(tmp);
        }
    }
    free(oldBuckets);
}

/* 添加操作 */
void putHashMapChaining(HashMapChaining* hmp, int key, const char* val)
{
    // 判断负载因子是否超过阈值
    if (loadFactor(hmp) > hmp->loadThres)
        extendHashMapChaining(hmp);
    
    // 遍历桶，若遇到指定 key ，则更新对应 val 并返回
    int index = hashFunc(hmp, key);
    Node* curr = hmp->buckets[index];
    while (curr)
    {
        if (curr->pair->key == key)
        {
            strcpy(curr->pair->val, val); // 若遇到指定 key ，则更新对应 val 并返回

            return;
        }
        curr = curr->next;
    }
    // 若无该 key ，则将键值对添加至链表头部
    Pair* newPair = malloc(sizeof(Pair)); // 创建新键值对
    newPair->key = key;
    newPair->val = malloc(strlen(val) + 1);
    strcpy(newPair->val, val);
    // 将新键值对添加到新节点, 并使新节点指向当前头节点
    Node* newNode = malloc(sizeof(Node));
    newNode->pair = newPair;
    newNode->next = hmp->buckets[index];
    // 将新节点添加到链表头部
    hmp->buckets[index] = newNode;
    hmp->size++;
}

/* 删除操作 */
void delItemHashMapChaining(HashMapChaining *hmp, int key)
{
    int index = hashFunc(hmp, key);
    // 遍历索引为index的桶内的链表
    Node* curr = hmp->buckets[index];
    Node* pre = NULL;
    while (curr)
    {
        if (curr->pair->key == key)
        {
            // 从中删除键值对
            if (pre)
                pre->next = curr->next;
            else
                hmp->buckets[index] = curr->next;
            // 释放内存
            free(curr->pair);
            free(curr);
            hmp->size--;
            return;
        }
        pre = curr;
        curr = curr->next;
    }
}

/* 打印哈希表 */
/* 打印哈希表 */
void printHashMapChaining(HashMapChaining* hmp)
{
    printf("哈希表内容:\n");
    for (int i = 0; i < hmp->capacity; i++)
    {
        printf("桶[%d]: ", i);
        Node* curr = hmp->buckets[i];
        if (!curr) {
            printf("空\n");
        } else {
            while (curr)
            {
                printf("{ %d: \"%s\" }", curr->pair->key, curr->pair->val);
                curr = curr->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}