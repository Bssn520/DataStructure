#include <stdio.h>
#include <stdlib.h>

/**
 * 二叉搜索树
 * 
 * 1. 查找节点
 * 给定目标节点值 num ，可以根据二叉搜索树的性质来查找。我们声明一个节点 cur ，从二叉树的根节点 root 出发，
 * 循环比较节点值 cur.val 和 num 之间的大小关系。
 * 
 * 若 cur.val < num ，说明目标节点在 cur 的右子树中，因此执行 cur = cur.right 。
 * 若 cur.val > num ，说明目标节点在 cur 的左子树中，因此执行 cur = cur.left 。
 * 若 cur.val = num ，说明找到目标节点，跳出循环并返回该节点。
 * 
 * 2. 插入节点
 * 给定一个待插入元素 num ，为了保持二叉搜索树“左子树 < 根节点 < 右子树”的性质, 流程如下:
 * (1). 查找插入位置：与查找操作相似，从根节点出发，根据当前节点值和 num 的大小关系循环向下搜索，直到越过叶节点（遍历至 None ）时跳出循环。
 * (2). 在该位置插入节点：初始化节点 num ，将该节点置于 None 的位置。
 * 
 * 3. 删除节点
 * (1). 当待删除节点的度为 0 时，表示该节点是叶节点，可以直接删除。
 * (2). 当待删除节点的度为 1 时，将待删除节点替换为其子节点即可。
 * (3). 当待删除节点的度为 2 时，我们无法直接删除它，而需要使用一个节点替换该节点。
 * 由于要保持二叉搜索树“左子树 < 根节点 < 右子树”的性质，因此这个节点可以是右子树的最小节点或左子树的最大节点。
 * 一般选择 "右子树的最小节点" , 它也是中序遍历的下一节点(二叉搜索树的中序遍历序列是升序的)。
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

/** 搜索二叉树节点结构体 */
typedef struct BinarySearchTree
{
    TreeNode* root;
} BinarySearchTree;

/* 构造函数 */
TreeNode* newTreeNode(int val);
/** 二分搜索树中查找节点 */
TreeNode* searchBinarySearchTree(BinarySearchTree* bst, int num);
/** 二分搜索树中插入节点 */
void insertBinarySearchTree(BinarySearchTree* bst, int num);
/** 二分搜索树中删除节点 */
void delBinarySearchTree(BinarySearchTree* bst, int num);
/* 打印中序遍历情况下的二叉树 */
void PrintInOrder(TreeNode* root);

int main()
{
    // 初始化二叉搜索树
    BinarySearchTree bst;
    bst.root = NULL;

    // 插入节点
    int nodes[] = { 50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45 };
    for (int i = 0; i < 11; i++)
    {
        insertBinarySearchTree(&bst, nodes[i]);
    }

    printf("二叉搜索树中序遍历结果：\n");
    PrintInOrder(bst.root);
    printf("\n");

    // 查找节点
    int target = 40;
    TreeNode* foundNode = searchBinarySearchTree(&bst, target);
    if (foundNode)
        printf("查找到节点: %d\n", foundNode->val);
    else
        printf("节点 %d 不存在\n", target);

    // 删除节点
    delBinarySearchTree(&bst, 20);
    printf("删除节点 20 后中序遍历结果：\n");
    PrintInOrder(bst.root);
    printf("\n");

    // 删除节点
    delBinarySearchTree(&bst, 30);
    printf("删除节点 30 后中序遍历结果：\n");
    PrintInOrder(bst.root);
    printf("\n");

    return 0;
}


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

/** 二分搜索树中查找节点 */
TreeNode* searchBinarySearchTree(BinarySearchTree* bst, int num)
{
    // 拿到根节点
    TreeNode* curr = bst->root;
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

/** 二分搜索树中插入节点 */
void insertBinarySearchTree(BinarySearchTree* bst, int num)
{
    // 若树为空, 则初始化根节点
    if (bst->root == NULL)
    {
        bst->root = newTreeNode(num);
        return;
    }

    // 查找插入位置, 会插入到叶节点后的位置
    TreeNode* curr = bst->root, *pre = NULL;
    while (curr != NULL)
    {
        // 如果发现重复节点, 则直接返回
        if (curr->val == num)
            return;
        
        pre = curr;
        if (curr->val < num)    // 如果num偏大, 则插入到右子树
            curr = curr->right;
        else
            curr = curr->left;  // 如果num偏小, 则插入到左子树
    }

    // 在找到的位置插入节点
    TreeNode* node  = newTreeNode(num);
    if (pre->val < num)
        pre->right = node;
    else
        pre->left = node;
}

/** 二分搜索树中删除节点 */
void delBinarySearchTree(BinarySearchTree* bst, int num)
{
    // 判断树是否为空
    if (bst->root == NULL)
        return;

    // 查找待删除节点
    TreeNode *curr = bst->root, *pre = NULL;
    while (curr != NULL)
    {
        // 如果发现重复节点, 则直接返回
        if (curr->val == num)
            break;;
        
        pre = curr;
        if (curr->val < num)    // 待删除节点在 root 的右子树中
            curr = curr->right;
        else
            curr = curr->left;  // 待删除节点在 root 的左子树中
    }

    // 如果没有待删除节点
    if (curr == NULL)
        return;

    // 判断待删除节点是否有子节点
    if (curr->left == NULL || curr->right == NULL)
    {
        /** 子节点数量为 0 or 1 */
        TreeNode* child = curr->left != NULL ? curr->left : curr->right;
        // 删除节点curr(令root指向 curr 的子节点即可)
        if (pre->left == curr)
            pre->left = child;
        else
            pre->right = child;
    }
    else
    {
        /** 子节点数量为 2 */
        // 获取中序遍历中curr的下一个节点
        TreeNode* tmp = curr->right;

        while (tmp->left != NULL)
            tmp = tmp->left;
        int tmpVal = tmp->val;
        // 递归删除节点tmp
        delBinarySearchTree(bst, tmpVal);
        // 用tmp覆盖curr
        curr->val = tmpVal;
    }
}

/* 中序遍历二叉树 */
void PrintInOrder(TreeNode* root)
{
    if (root == NULL)
        return;
    PrintInOrder(root->left);
    printf("%d ", root->val);
    PrintInOrder(root->right);
}