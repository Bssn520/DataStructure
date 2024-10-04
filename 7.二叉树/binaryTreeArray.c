#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 二叉树的数组表示
 * 
 * 
 * 根据层序遍历的特性，我们可以推导出父节点索引与子节点索引之间的“映射公式”：若某节点的索引为 i ,
 * 则该节点的左子节点索引为 2 * i + 1, 右子节点索引为 2 * i + 2 
 * 映射公式的角色相当于链表中的节点引用（指针）。给定数组中的任意一个节点，我们都可以通过映射公式来访问它的左（右）子节点。
 * 
 * 为了解决此问题，我们可以考虑在层序遍历序列中显式地写出所有 None 。这样处理后，层序遍历序列就可以唯一表示二叉树了
 * 使用 int 最大值标记空位，因此要求节点值不能为 INT_MAX: 
 * int tree[] = {1, 2, 3, 4, INT_MAX, 6, 7, 8, 9, INT_MAX, INT_MAX, 12, INT_MAX, INT_MAX, 15};
 */

/** 数组表示下的二叉树结构体 */
typedef struct ArrayBinaryTree
{
    int* tree;
    int size;
} ArrayBinaryTree;

/** 构造函数 */
ArrayBinaryTree* newArrayBinaryTree(int* arr, int arrSize)
{
    ArrayBinaryTree* abt = (ArrayBinaryTree*)malloc(sizeof(ArrayBinaryTree));
    // 为 abt 结构体中的 tree 成员分配足够大小的内存空间，用于保存数组中的元素。
    abt->tree = malloc(sizeof(int) * arrSize);
    // 将传入的 arr 数组中的所有元素复制到 abt->tree 中。
    memcpy(abt->tree, arr, sizeof(int) * arrSize);
    abt->size = arrSize;

    return abt;
}

/** 析构函数 */
void destroyArrayBinaryTree(ArrayBinaryTree* abt)
{
    free(abt->tree);
    free(abt);
}

/** 列表容量 */
int sizeArrayBinaryTree(ArrayBinaryTree* abt)
{
    return abt->size;
}

/** 获取索引为 i 的节点的值 */
int getValArrayBinaryTree(ArrayBinaryTree* abt, int index)
{
    // 若索引越界, 则返回 INT8_MAX
    if (index < 0 || index >= sizeArrayBinaryTree(abt))
        return INT8_MAX;
    
    return abt->tree[index];
}

/** 层序遍历 */
int* levelOrder(ArrayBinaryTree* abt, int* returnSize)
{
    int* res = (int*)malloc(sizeof(int) * sizeArrayBinaryTree(abt));
    // 遍历数组
    int index = 0;
    for (int i = 0; i < sizeArrayBinaryTree(abt); i++)
    {
        if (getValArrayBinaryTree(abt, i) != INT8_MAX)
            res[index++] = abt->tree[i];
    }
    *returnSize = index;
    
    return res;
}

/** 左子节点索引 */
int left(int i)
{
    return (2 * i) + 1;
}
/** 右子节点索引 */
int right(int i)
{
    return (2 * i) + 2;
}

/** 深度优先遍历 */
void dfs(ArrayBinaryTree* abt, int i, char* order, int* res, int* index)
{
    // 若为空，则返回
    if (getValArrayBinaryTree(abt, i) == INT8_MAX)
        return;
    // 前序遍历
    if (strcmp(order, "pre") == 0)  // 如果是前序遍历, 正好符合 根 --> 左 --> 右
        res[(*index)++] = getValArrayBinaryTree(abt, i);    // 记录 根节点
    dfs(abt, left(i), order, res, index);   // 遍历左子树
    // 中序遍历
    if (strcmp(order, "in") == 0)   // 如果是中序遍历, 此前已经遍历了左子树, 正好符合 左 --> 根 --> 右
        res[(*index)++] = getValArrayBinaryTree(abt, i);
    dfs(abt, right(i), order, res, index);  // 遍历右子树
    // 后序遍历
    if (strcmp(order, "post") == 0)     // 如果是后序遍历, 此前已经遍历了左右子树, 正好符合 左 --> 右 --> 根
        res[(*index)++] = getValArrayBinaryTree(abt, i);
}

/** 前序遍历 */
int* preOrder(ArrayBinaryTree* abt, int* returnSize)
{
    int* res = (int*)malloc(sizeof(int) * sizeArrayBinaryTree(abt));
    int index = 0;
    dfs(abt, 0, "pre", res, &index);
    *returnSize = index;

    return res;
}

/** 中序遍历 */
int* inOrder(ArrayBinaryTree* abt, int* returnSize)
{
    int* res = (int*)malloc(sizeof(int) * sizeArrayBinaryTree(abt));
    int index = 0;
    dfs(abt, 0, "in", res, &index);
    *returnSize = index;

    return res; 
}

/** 后序遍历 */
int* postOrder(ArrayBinaryTree* abt, int* returnSize)
{
    int* res = (int*)malloc(sizeof(int) * sizeArrayBinaryTree(abt));
    int index = 0;
    dfs(abt, 0, "post", res, &index);
    *returnSize = index;

    return res;
}


int main()
{
    // 构造一个非完美二叉树，使用数组表示法
    int arr[] = {0, 7, 2, 1, INT8_MAX, 5, 6, 3, 4}; // 使用 int 最大值标记空位，因此要求节点值不能为 INT8_MAX
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    // 创建一个数组表示的二叉树
    ArrayBinaryTree* abt = newArrayBinaryTree(arr, arrSize);

    // 存储返回的结果大小
    int returnSize;

    // 前序遍历
    int* preRes = preOrder(abt, &returnSize);
    printf("前序遍历: ");
    for (int i = 0; i < returnSize; i++)
        printf("%d ", preRes[i]);
    printf("\n");

    // 中序遍历
    int* inRes = inOrder(abt, &returnSize);
    printf("中序遍历: ");
    for (int i = 0; i < returnSize; i++)
        printf("%d ", inRes[i]);
    printf("\n");

    // 后序遍历
    int* postRes = postOrder(abt, &returnSize);
    printf("后序遍历: ");
    for (int i = 0; i < returnSize; i++)
        printf("%d ", postRes[i]);
    printf("\n");

    // 层序遍历
    int* levelRes = levelOrder(abt, &returnSize);
    printf("层序遍历: ");
    for (int i = 0; i < returnSize; i++)
        printf("%d ", levelRes[i]);
    printf("\n");

    // 释放所有动态分配的内存
    free(preRes);
    free(inRes);
    free(postRes);
    free(levelRes);
    destroyArrayBinaryTree(abt);

    return 0;
}