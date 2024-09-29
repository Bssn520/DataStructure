#include <stdio.h>
#include <stdlib.h>

/**
 * 二叉树
 * 
 * 二叉树（binary tree）是一种非线性数据结构，代表“祖先”与“后代”之间的派生关系，体现了“一分为二”的分治逻辑。
 * 与链表类似，二叉树的基本单元是节点，每个节点包含值、左子节点引用和右子节点引用。
 * 
 * 左右子树:
 * 每个节点都有两个引用（指针），分别指向左子节点和右子节点，该节点被称为这两个子节点的父节点。
 * 当给定一个二叉树的节点时，我们将该节点的左子节点及其以下节点形成的树称为该节点的左子树，同理可得右子树。
 * 在二叉树中，除叶节点外，其他所有节点都包含子节点和非空子树。
 * 
 * 常见术语:
 * 根节点（root node）：位于二叉树顶层的节点，没有父节点。
 * 叶节点（leaf node）：没有子节点的节点，其两个指针均指向 None 。
 * 边（edge）：连接两个节点的线段，即节点引用（指针）。
 * 节点所在的层（level）：从顶至底递增，根节点所在层为 1 。
 * 节点的度（degree）：节点的子节点的数量。在二叉树中，度的取值范围是 0、1、2 。
 * 二叉树的高度（height）：从根节点到最远叶节点所经过的边的数量。
 * 节点的深度（depth）：从根节点到该节点所经过的边的数量。
 * 节点的高度（height）：从距离该节点最远的叶节点到该节点所经过的边的数量。
 * 请注意，我们通常将“高度”和“深度”定义为“经过的边的数量”，但有些题目或教材可能会将其定义为“经过的节点的数量”。在这种情况下，高度和深度都需要加 1 。
 * 
 * 常见二叉树类型:
 * 
 * 完美二叉树:
 * 完美二叉树（perfect binary tree）所有层的节点都被完全填满。在完美二叉树中，叶节点的度为 0, 其余所有节点的度都为 2;
 * 若树的高度为 h, 则节点总数为 2^(h+1) - 1, 呈现标准的指数级关系，反映了自然界中常见的细胞分裂现象。
 * 
 * 完全二叉树:
 * 完全二叉树只有最底层的节点未被填满，且最底层节点尽量靠左填充。请注意，完美二叉树也是一棵完全二叉树。
 * 
 * 完满二叉树:
 * 完满二叉树除了叶节点之外，其余所有节点都有两个子节点。
 * 
 * 平衡二叉树:
 * 平衡二叉树中任意节点的左子树和右子树的高度之差的绝对值不超过 1 。
 * 
 * 二叉树的退化:
 * 当二叉树的每层节点都被填满时，达到“完美二叉树”；而当所有节点都偏向一侧时，二叉树退化为“链表”。
 * 完美二叉树是理想情况，可以充分发挥二叉树“分治”的优势。
 * 链表则是另一个极端，各项操作都变为线性操作，时间复杂度退化至 O(n)。
 * 在最佳结构和最差结构下，二叉树的叶节点数量、节点总数、高度等达到极大值或极小值:
 *                          完美二叉树              链表
 * 第 i 层的节点数量           2^i - 1               1
 * 高度为 h 的树的叶节点数量	2^h                   1
 * 高度为 h 的树的节点总数	    2^(h+1) - 1           h + 1
 * 节点总数为 n 的树的高度      [log2(n + 1)] - 1     n - 1
 * 
 */

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
    // 删除节点P
    node0->left = node1;
}
