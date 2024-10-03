#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 10

/* 二叉树节点结构体 */
typedef struct TreeNode
{
    int val;            // 节点值
    int height;         // 节点高度
    struct TreeNode* left;     // 左子节点指针
    struct TreeNode* right;    // 右子节点指针
} TreeNode;

/* 构造函数 */
TreeNode* newTreeNode(int val)
{
    TreeNode* node = malloc(sizeof(TreeNode));
    node->val = val;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/** 层序遍历 */
int* levelOrder(TreeNode* root, int* size);
/* 前序遍历 */
void preOrder(TreeNode *root, int *size, int* arr);
/* 中序遍历 */
void inOrder(TreeNode *root, int *size, int* arr);
/* 后序遍历 */
void postOrder(TreeNode *root, int *size, int* arr);
/** 打印遍历结果 */
void printOrder(int* arr, int size);


/**
 * 二叉树的遍历
 * 
 * 1.层序遍历
 * 按层从上到下进行遍历。
 * 2. 前序遍历
 * 若二叉树为空，则空操作返回，否则先访问根结点 ，然后前序遍历左子树，再前序遍历右子树。
 * 3.中序遍历
 * 若树为空，则空操作返回，否则从根结点开始（注意并不是先访问根结点），中序遍历根结点的左子树，然后是访问根结点，最后中序遍历右子树。
 * 4.后序遍历
 * 若树为空，则空操作返回，否则以左到右先叶子后结点的方式遍历访问左右子树，最后访问根结点。
 * 
 */

int main()
{
    /* 初始化二叉树 */
    // 创初始化节点
    TreeNode* node0 = newTreeNode(0);
    TreeNode* node1 = newTreeNode(1);
    TreeNode* node2 = newTreeNode(2);
    TreeNode* node3 = newTreeNode(3);
    TreeNode* node4 = newTreeNode(4);
    TreeNode* node5 = newTreeNode(5);
    TreeNode* node6 = newTreeNode(6);
    
    // 构建节点间的引用
    node0->left = node1;
    node0->right = node2;
    node1->left = node3;
    node1->right = node4;
    node2->left = node5;
    node2->right = node6;

    /* 插入与删除节点 */
    TreeNode* P = newTreeNode(7);
    // 在node0 与 node1 之间插入节点P
    node0->left = P;
    P->left = node1;


    // 定义辅助变量存储数据
    int size = 0;
    int* result = malloc(sizeof(int) * MAX_SIZE);

    /**
     * 层序遍历测试
     */
    // 输出层序遍历的结果
    // 插入P前: 0 1 2 3 4 5 6
    // 插入P后: 0 7 2 1 5 6 3 4
    printf("层序遍历结果: ");
    result = levelOrder(node0, &size);
    printOrder(result, size);
    free(result);

    /**
     * 前序遍历测试
     */
    size = 0;
    result = malloc(sizeof(int) * MAX_SIZE);
    printf("前序遍历结果: ");
    preOrder(node0, &size, result);
    printOrder(result, size);
    free(result);

    /**
     * 中序遍历测试
     */
    size = 0;
    result = malloc(sizeof(int) * MAX_SIZE);
    printf("中序遍历结果: ");
    inOrder(node0, &size, result);
    printOrder(result, size);
    free(result);

    /**
     * 后序遍历测试
     */
    size = 0;
    result = malloc(sizeof(int) * MAX_SIZE);
    printf("后序遍历结果: ");
    postOrder(node0, &size, result);
    printOrder(result, size);
    free(result);

    return 0;
}

/** 层序遍历 */
int* levelOrder(TreeNode* root, int* size)
{
    /** 辅助队列 */
    int front, rear;
    int index, *arr;
    TreeNode* node;
    TreeNode** queue;   // 指针: 指向一个TreeNode类型的指针

    /** 辅助队列 */
    queue = (TreeNode**)malloc(sizeof(TreeNode) * MAX_SIZE);

    /** 队列指针 */
    front = 0, rear = 0;

    /** 入队根节点 */
    queue[rear++] = root;

    /** 初始化辅助数组，用于保存遍历序列 */
    arr = (int*)malloc(sizeof(int) * MAX_SIZE);
    // 数组指针
    index = 0;
    while (front < rear)    // front < rear 表示队列中还有节点未被处理
    {
        // 队列出队
        node = queue[front++];
        // 保存节点值
        arr[index++] = node->val;
        // 左节点入队
        if (node->left != NULL)
            queue[rear++] = node->left;
        // 右节点入队
        if (node->right != NULL)
            queue[rear++] = node->right;
    }
    // 更新数组长度的值
    *size = index;
    arr = realloc(arr, sizeof(int) * (*size));

    // 释放辅助数组
    free(queue);

    return arr;
}

/* 前序遍历 */
void preOrder(TreeNode *root, int *size, int* arr)
{
    if (root == NULL)
        return;
    // 优先级: 根节点 --> 左子树 --> 右子树
    arr[(*size)++] = root->val;
    preOrder(root->left, size, arr);
    preOrder(root->right, size, arr);
}

/* 中序遍历 */
void inOrder(TreeNode *root, int *size, int* arr)
{
    if (root == NULL)
        return;
    // 优先级: 左子树 --> 根节点 --> 右子树
    inOrder(root->left, size, arr);
    arr[(*size)++] = root->val;
    inOrder(root->right, size, arr);
}

/* 后序遍历 */
void postOrder(TreeNode *root, int *size, int* arr)
{
    if (root == NULL)
        return;
    // 优先级: 左子树 --> 右子树 --> 根节点
    postOrder(root->left, size, arr);
    postOrder(root->right, size, arr);
    arr[(*size)++] = root->val;
}

/** 打印遍历结果 */
void printOrder(int* arr, int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}