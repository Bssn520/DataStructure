#include <stdio.h>
#include <stdlib.h>

/**
 * AVL树
 * 
 * AVL树在持续添加和删除节点后不会退化, 从而使得各种操作的时间复杂度保持在 O(log n) 级别。
 * AVL 树既是二叉搜索树，也是平衡二叉树，同时满足这两类二叉树的所有性质，因此是一种平衡二叉搜索树。
 * 
 * 节点高度: 是指从该节点到它的最远叶节点的距离，即所经过的“边”的数量。需要特别注意的是，叶节点的高度为 0，而空节点的高度为 -1
 * 节点的平衡因子: 定义为节点左子树的高度减去右子树的高度，同时规定空节点的平衡因子为 0
 * 
 * AVL 树的特点在于“旋转”操作，它能够在不影响二叉树的中序遍历序列的前提下，使失衡节点重新恢复平衡。
 * 换句话说，旋转操作既能保持“二叉搜索树”的性质，也能使树重新变为“平衡二叉树”。
 * 1. 右旋
 * 我们关注以失衡节点为根节点的子树，将该节点记为 node ，其左子节点记为 child ，执行“右旋”操作:
 * (1). 以 child 为原点， node 向右旋转。
 * (2). 用 child 替代以前 node 的位置。
 * 当节点 child 有右子节点（记为 grand_child ）时，需要在右旋中添加一步：将 grand_child 作为 node 的左子节点。
 * 2. 左旋
 * 右旋和左旋操作在逻辑上是镜像对称的，它们分别解决的两种失衡情况也是对称的。
 * 基于对称性，我们只需将右旋的实现代码中的所有的 left 替换为 right ，将所有的 right 替换为 left ，即可得到左旋的实现代码。
 * 3. 先左旋后右旋
 * 先对 child 执行“左旋”，再对 node 执行“右旋”。
 * 4. 先右旋后左旋
 * 需要先对 child 执行“右旋”，再对 node 执行“左旋”。
 * 
 * 旋转的选择:
 * 我们通过判断失衡节点的平衡因子以及较高一侧子节点的平衡因子的正负号，来确定失衡节点属于哪种情况:
 * 失衡节点的平衡因子       子节点的平衡因子        应采用的旋转方法
 * > 1 (左偏树)             >= 0                 右旋
 * > 1 (左偏树)             < 0                  先左旋后右旋
 * < -1 (右偏树)            <= 0                 左旋
 * < -1 (右偏树)            > 0                  先右旋后左旋
 * 
 * 插入节点:
 * AVL 树的节点插入操作与二叉搜索树在主体上类似。
 * 唯一的区别在于，在 AVL 树中插入节点后，从该节点到根节点的路径上可能会出现一系列失衡节点。
 * 因此，我们需要从这个节点开始，自底向上执行旋转操作，使所有失衡节点恢复平衡。
 * 
 * 删除节点:
 * 类似地，在二叉搜索树的删除节点方法的基础上，需要从底至顶执行旋转操作，使所有失衡节点恢复平衡。
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

/** AVL 树结构体 */
typedef struct AVLTree
{
    TreeNode* root;
} AVLTree;


/* 树节点构造函数 */
TreeNode* newTreeNode(int val);
/** 获取节点高度 */
int heightTreeNode(TreeNode* node);
/** 更新节点高度 */
void heightUpdate(TreeNode* node);
/** 获取节点平衡因子 */
int balanceFactor(TreeNode* node);
/** 右旋 */
TreeNode* rightRotate(TreeNode* node);
/** 左旋 */
TreeNode* leftRotate(TreeNode* node);
/* 执行旋转操作，使子树重新恢复平衡 */
TreeNode* rotate(TreeNode* node);
/* 递归插入节点（辅助函数） */
TreeNode* insertHelper(TreeNode *node, int val);
/** 插入节点 */
void insertAVLTree(AVLTree* tree, int val);
/* 递归删除节点（辅助函数） */
TreeNode *removeHelper(TreeNode *node, int val);
/* 删除节点 */
void removeItem(AVLTree *tree, int val);
/** 查找节点 */
TreeNode* searchAVLTree(AVLTree* tree, int num);


/* 辅助函数：前序遍历 */
void preOrder(TreeNode* root)
{
    if (root != NULL)
    {
        printf("%d ", root->val);
        preOrder(root->left);
        preOrder(root->right);
    }
}

/* 辅助函数：中序遍历 */
void inOrder(TreeNode* root)
{
    if (root != NULL)
    {
        inOrder(root->left);
        printf("%d ", root->val);
        inOrder(root->right);
    }
}

/* 辅助函数：后序遍历 */
void postOrder(TreeNode* root)
{
    if (root != NULL)
    {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->val);
    }
}

/* 主函数：测试 AVL 树操作 */
int main()
{
    // 创建 AVL 树
    AVLTree* tree = malloc(sizeof(AVLTree));
    tree->root = NULL;

    // 插入一些节点
    int nums[] = { 50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45 };
    for (int i = 0; i < 11; i++)
    {
        insertAVLTree(tree, nums[i]);
    }

    // 打印树的前序遍历
    printf("前序遍历: ");
    preOrder(tree->root);
    printf("\n");

    // 打印树的中序遍历
    printf("中序遍历: ");
    inOrder(tree->root);
    printf("\n");

    // 打印树的后序遍历
    printf("后序遍历: ");
    postOrder(tree->root);
    printf("\n");

    // 查找节点
    int searchVal = 25;
    TreeNode* searchNode = searchAVLTree(tree, searchVal);
    if (searchNode != NULL)
    {
        printf("查找节点 %d: 找到，值为 %d\n", searchVal, searchNode->val);
    }
    else
    {
        printf("查找节点 %d: 未找到\n", searchVal);
    }

    // 删除节点
    int deleteVal = 30;
    printf("删除节点 %d:\n", deleteVal);
    removeItem(tree, deleteVal);

    // 打印删除后的树的前序遍历
    printf("删除后前序遍历: ");
    preOrder(tree->root);
    printf("\n");

    // 打印删除后的树的中序遍历
    printf("删除后中序遍历: ");
    inOrder(tree->root);
    printf("\n");

    // 释放内存
    free(tree);

    return 0;
}


/* 树节点构造函数 */
TreeNode* newTreeNode(int val)
{
    TreeNode* node = malloc(sizeof(TreeNode));
    node->val = val;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/** 获取节点高度 */
int heightTreeNode(TreeNode* node)
{
    // 空节点高度为 -1 ，叶节点高度为 0
    if (node == NULL)
        return -1;

    return node->height;
}

/** 更新节点高度 */
void heightUpdate(TreeNode* node)
{
    int lh = heightTreeNode(node->left);
    int rh = heightTreeNode(node->right);
    // 节点高度等于最高子树高度 + 1
    node->height = (lh > rh ? lh : rh) + 1;
}

/** 获取节点平衡因子 */
int balanceFactor(TreeNode* node)
{
    // 空节点平衡因子为0
    if (!node)
        return 0;
    // 节点平衡因子 = 左子树高度 - 右子树高度
    int height = heightTreeNode(node->left) - heightTreeNode(node->right);
    return height;
}

/** 右旋 */
TreeNode* rightRotate(TreeNode* node)
{
    // 检查节点是否为空
    if (node == NULL || node->left == NULL) return node;

    TreeNode* child = node->left;
    TreeNode* grandChild = child->right;

    // 以child为原点, 将node向右旋转
    child->right = node;
    node->left = grandChild;

    // 更新节点高度
    heightUpdate(node);
    heightUpdate(child);

    // 返回旋转后子树的根节点
    return child;  
}

/** 左旋 */
TreeNode* leftRotate(TreeNode* node)
{
    // 检查节点是否为空
    if (node == NULL || node->right == NULL) return node;

    TreeNode* child = node->right;
    TreeNode* grandChild = child->left;

    // 以child为原点, 将node向左旋转
    child->left = node;
    node->right = grandChild;

    // 更新节点高度
    heightUpdate(node);
    heightUpdate(child);

    // 返回旋转后子树的根节点
    return child;  
}

/* 执行旋转操作，使子树重新恢复平衡 */
TreeNode* rotate(TreeNode* node)
{
    // 获取该节点的平衡因子
    int bf = balanceFactor(node);

    // 左偏树
    if (bf > 1)
    {
        if (balanceFactor(node->left) >= 0)     // 左子树左偏
        {
            // 右旋
            return rightRotate(node);
        }
        else    // 左子树右偏
        {
            // 先左旋再右旋
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }

    // 右偏树
    if (bf < -1)
    {
        if (balanceFactor(node->right) <= 0)    // 右子树右偏
        {
            // 左旋
            return leftRotate(node);
        }
        else    // 右子树左偏
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    // 平衡树，无须旋转，直接返回
    return node;
}

/* 递归插入节点（辅助函数） */
TreeNode* insertHelper(TreeNode *node, int val)
{
    if (node == NULL) return newTreeNode(val);

    // 查找插入位置并插入节点
    if (node->val < val)
        node->right = insertHelper(node->right, val);
    else if (node->val > val)
        node->left = insertHelper(node->left, val);
    else    // 重复节点，直接返回
        return node;

    // 更新节点高度
    heightUpdate(node);

    // 执行旋转操作，使该子树恢复平衡
    node = rotate(node);

    // 返回子树的根节点
    return node;
}

/** 插入节点 */
void insertAVLTree(AVLTree* tree, int val)
{
    tree->root = insertHelper(tree->root, val);
}

/* 递归删除节点（辅助函数） */
TreeNode *removeHelper(TreeNode *node, int val)
{
    TreeNode *child, *grandChild;
    if (node == NULL) {
        return NULL;
    }
    /* 1. 查找节点并删除 */
    if (val < node->val) {
        node->left = removeHelper(node->left, val);
    } else if (val > node->val) {
        node->right = removeHelper(node->right, val);
    } else {
        if (node->left == NULL || node->right == NULL) {
            child = node->left;
            if (node->right != NULL) {
                child = node->right;
            }
            // 子节点数量 = 0 ，直接删除 node 并返回
            if (child == NULL) {
                return NULL;
            } else {
                // 子节点数量 = 1 ，直接删除 node
                node = child;
            }
        } else {
            // 子节点数量 = 2 ，则将中序遍历的下个节点删除，并用该节点替换当前节点
            TreeNode *temp = node->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            int tempVal = temp->val;
            node->right = removeHelper(node->right, temp->val);
            node->val = tempVal;
        }
    }
    // 更新节点高度
    heightUpdate(node);
    /* 2. 执行旋转操作，使该子树重新恢复平衡 */
    // 返回子树的根节点
    return rotate(node);
}

/* 删除节点 */
void removeItem(AVLTree *tree, int val)
{
    TreeNode* root = removeHelper(tree->root, val);
}

/** 查找节点 */
TreeNode* searchAVLTree(AVLTree* tree, int num)
{
    // 拿到根节点
    TreeNode* curr = tree->root;
    // 循环查找, 越过叶节点后跳出
    while (curr != NULL)
    {
        if (curr->val < num)        // 如果 num 大于 curr, 则 num 位于右子树
            curr = curr->right;
        else if (curr->val > num)   // // 如果 num 小于 curr, 则 num 位于左子树
            curr = curr->left;
        else
            break;  // 找到目标节点，跳出循环
    }
    // 返回目标节点
    return curr;
}