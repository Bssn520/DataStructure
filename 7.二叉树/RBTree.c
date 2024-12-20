/* 来源：https://developer.aliyun.com/article/1424813 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 1
#define BLACK 2

#define ENABLE_TYPE_CHAR 1

#if ENABLE_TYPE_CHAR
typedef char *KEY_TYPE;
#else
typedef int KEY_TYPE;
#endif

typedef struct _rbtree_node
{
    unsigned char color;
    struct _rbtree_node *right;
    struct _rbtree_node *left;
    struct _rbtree_node *parent; // 旋转时需要用到父结点的指针
    KEY_TYPE key;
    void *value;
} rbtree_node;

typedef struct _rbtree
{
    rbtree_node *root;
    // 公用的nil节点-可以理解为隐藏的叶子节点
    // （也可以理解为一个万能的"空"节点，
    // 所有节点的3个指针没有具体指向时，需要指向这个节点
    rbtree_node *nil;
} rbtree;

rbtree_node *rbtree_mini(rbtree *T, rbtree_node *x)
{
    while (x->left != T->nil)
    {
        x = x->left;
    }
    return x;
}

rbtree_node *rbtree_maxi(rbtree *T, rbtree_node *x)
{
    while (x->right != T->nil)
    {
        x = x->right;
    }
    return x;
}

rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x)
{
    rbtree_node *y = x->parent;
    if (x->right != T->nil)
    {
        return rbtree_mini(T, x->right);
    }
    while ((y != T->nil) && (x == y->right))
    {
        x = y;
        y = y->parent;
    }
    return y;
}

// 左旋-插入和删除时调整树
void rbtree_left_rotate(rbtree *T, rbtree_node *x)
{
    rbtree_node *y = x->right;
    x->right = y->left; // 1 1  x的右指针设置为右孩子的左指针
    if (y->left != T->nil)
    { // 1 2
        y->left->parent = x;
    }
    y->parent = x->parent; // 1 3
    if (x->parent == T->nil)
    { // 1 4
        T->root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x; // 1 5
    x->parent = y; // 1 6
}

// 右旋-插入和删除时调整树（和左旋的实现的代码区别，
//  x变成y，y变成x，left变成right，right变成left）
void rbtree_right_rotate(rbtree *T, rbtree_node *y)
{
    rbtree_node *x = y->left;
    y->left = x->right;
    if (x->right != T->nil)
    {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == T->nil)
    {
        T->root = x;
    }
    else if (y == y->parent->right)
    {
        y->parent->right = x;
    }
    else
    {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// 插入修正处理-为了保持红黑树的性质（要保证插入之后也是一棵红黑树）
// 要想理解这个过程，请自己画图表示-重点是理解红黑树性质
void rbtree_insert_fixup(rbtree *T, rbtree_node *z)
{
    // 为什么需要while? 从insert调用的第一步可以先当成if来理解
    // 然后看后面有z---->RED代表递归处理祖父节点。
    while (z->parent->color == RED)
    { // z ---> RED ，父亲是红色违背了红黑树的性质4
        if (z->parent == z->parent->parent->left)
        { // 父亲节点是祖父节点的左孩子
            rbtree_node *y = z->parent->parent->right;
            if (y->color == RED)
            { // 叔父节点是红色，这种情况不需要旋转，
                z->parent->color = BLACK; //
                y->color = BLACK;
                z->parent->parent->color = RED; // 注意，这里修改了祖父的颜色为红

                // 这里为什么将z设置为z的祖父，然后进行循环处理呢？因为上面一句代码将祖父设置为了红，那么祖父的父亲
                // 会不会也为红呢？是不是又违背了红黑树的性质4呢。那么指导为什么要使用while循环递归处理了吧
                z = z->parent->parent; // z --> RED,
            }
            else
            { // 叔叔是黑色
                if (z == z->parent->right)
                { // 当前节点是是父亲右孩子,将当前节点旋转到和父亲与祖父都在左子树中
                    z = z->parent; // 将“父节点”作为“新的当前节点”。
                    rbtree_left_rotate(T, z); // 以“新的当前节点”为支点进行左旋。
                }
                z->parent->color = BLACK; // 插入的节点变为黑色
                z->parent->parent->color = RED; // 祖父节点变为红色
                // 祖父节点为基点，右旋，此时插入的节点为父亲节点、祖父节点的父亲，父亲为左节点，祖父变成右节点。
                rbtree_right_rotate(T, z->parent->parent);
            }
        }
        else
        { // 父亲节点是祖父节点的左孩子，类似上面的分析
            rbtree_node *y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent; // z --> RED
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rbtree_right_rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rbtree_left_rotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

// 代码的详细说明在博客的引文都有详细说明
void rbtree_insert(rbtree *T, rbtree_node *z)
{
#if ENABLE_TYPE_CHAR

    rbtree_node *y = T->nil; // 新建节点y，初始设置为nil节点（表示插入节点的父亲节点）
    rbtree_node *x = T->root; //
    while (x != T->nil)
    { // 藏根节点循环找到要插入的位置
        y = x;
        if (strcmp(z->key, x->key) < 0)
        { // 插入的节点值在树的左边
            x = x->left;
        }
        else if (strcmp(z->key, x->key) > 0)
        {
            x = x->right;
        }
        // Exist，插入的节点的key, 红黑树已经存在对应的key，
        // 不做任何操作，这个要看具体的逻辑，比如在定时器来串联任务时，
        // 用红黑树来表示多个定期器时，这就相当于2个任务在同一个时间
        // 执行，这时候如果不插入，那么就相当于丢掉了一个任务。
        // 实现时，需要将定时器的key+一个很小值，或者key减去一个很小值，
        // 然后在进行插入。
        else
        {
            return;
        }
    }
    z->parent = y;
    if (y == T->nil)
    { // 要插入的节点的父亲节点为NULL,将z设置为根节点
        T->root = z; //
    }
    else if (strcmp(z->key, y->key) < 0)
    { // 若“z所包含的值” < “z父亲所包含的值”，则将z设为“z父亲的左孩子”
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    z->left = T->nil; // 左右指向公共的节点
    z->right = T->nil;
    z->color = RED; // 默认为红色(请参考博客引文说明为啥需要为红色)
    rbtree_insert_fixup(T, z);

#else

    rbtree_node *y = T->nil; // 新建节点y，初始设置为nil节点（表示插入节点的父亲节点）
    rbtree_node *x = T->root; //
    while (x != T->nil)
    { // 藏根节点循环找到要插入的位置
        y = x;
        if (z->key < x->key)
        { // 插入的节点值在树的左边
            x = x->left;
        }
        else if (z->key > x->key)
        {
            x = x->right;
        }
        // Exist，插入的节点的key, 红黑树已经存在对应的key，
        // 不做任何操作，这个要看具体的逻辑，比如在定时器来串联任务时，
        // 用红黑树来表示多个定期器时，这就相当于2个任务在同一个时间
        // 执行，这时候如果不插入，那么就相当于丢掉了一个任务。
        // 实现时，需要将定时器的key+一个很小值，或者key减去一个很小值，
        // 然后在进行插入。
        else
        {
            return;
        }
    }
    z->parent = y;
    if (y == T->nil)
    { // 要插入的节点的父亲节点为NULL,将z设置为根节点
        T->root = z; //
    }
    else if (z->key < y->key)
    { // 若“z所包含的值” < “z父亲所包含的值”，则将z设为“z父亲的左孩子”
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    z->left = T->nil; // 左右指向公共的节点
    z->right = T->nil;
    z->color = RED; // 默认为红色(请参考博客引文说明为啥需要为红色)
    rbtree_insert_fixup(T, z);

#endif
}

void rbtree_delete_fixup(rbtree *T, rbtree_node *x)
{
    while ((x != T->root) && (x->color == BLACK))
    {
        if (x == x->parent->left)
        {
            rbtree_node *w = x->parent->right;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_left_rotate(T, x->parent);
                w = x->parent->right;
            }
            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rbtree_right_rotate(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rbtree_left_rotate(T, x->parent);
                x = T->root;
            }
        }
        else
        {
            rbtree_node *w = x->parent->left;
            if (w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rbtree_right_rotate(T, x->parent);
                w = x->parent->left;
            }
            if ((w->left->color == BLACK) && (w->right->color == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    rbtree_left_rotate(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rbtree_right_rotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z)
{
    rbtree_node *y = T->nil;
    rbtree_node *x = T->nil;
    if ((z->left == T->nil) || (z->right == T->nil))
    {
        y = z;
    }
    else
    {
        y = rbtree_successor(T, z);
    }
    if (y->left != T->nil)
    {
        x = y->left;
    }
    else if (y->right != T->nil)
    {
        x = y->right;
    }
    x->parent = y->parent;
    if (y->parent == T->nil)
    {
        T->root = x;
    }
    else if (y == y->parent->left)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }
    if (y != z)
    {
#if ENABLE_TYPE_CHAR
        void *tmp = z->key;

        z->key = y->key;
        y->key = tmp;

        tmp = z->value;
        z->value = y->value;
        y->value = tmp;
#else
        z->key = y->key;
        z->value = y->value;
#endif
    }
    if (y->color == BLACK)
    {
        rbtree_delete_fixup(T, x);
    }
    return y;
}

rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key)
{
    rbtree_node *node = T->root;
    while (node != T->nil)
    {
#if ENABLE_TYPE_CHAR
        if (strcmp(key, node->key) < 0)
        {
            node = node->left;
        }
        else if (strcmp(key, node->key) > 0)
        {
            node = node->right;
        }
        else
        {
            return node;
        }
#else
        if (key < node->key)
        {
            node = node->left;
        }
        else if (key > node->key)
        {
            node = node->right;
        }
        else
        {
            return node;
        }
#endif
    }
    return T->nil;
}

void rbtree_traversal(rbtree *T, rbtree_node *node)
{
    if (node != T->nil)
    {
        rbtree_traversal(T, node->left);
#if ENABLE_TYPE_CHAR
        printf("%s : %s\n", node->key, (char *)node->value);
#else
        printf("key:%d, color:%d\n", node->key, node->color);
#endif
        rbtree_traversal(T, node->right);
    }
}

int main()
{
#if ENABLE_TYPE_CHAR
    KEY_TYPE keyArray[10] = {"Abc", "Sjks", "NNSkq", "Kksldm", "Jalk", "Hello", "Hiiii", "Nihao", "Wobuhao", "112233"};
    KEY_TYPE valArray[10] = {"abc", "sjks", "nNSkq", "kksldm", "jalk", "hello", "hiiii", "nihao", "wobuhao", "112233"};

    rbtree *T = (rbtree *)malloc(sizeof(rbtree));
    if (T == NULL)
    {
        printf("malloc failed\n");
        return -1;
    }
    // 初始化nil节点
    T->nil = (rbtree_node *)malloc(sizeof(rbtree_node));
    T->nil->color = BLACK; // 默认nil的节点为黑色，红黑树性质3
    T->root = T->nil; // root默认指向nil，代表还没有结点

    rbtree_node *node = T->nil;

    int i = 0;
    for (i = 0; i < 10; i++)
    {
        node = (rbtree_node *)malloc(sizeof(rbtree_node)); // 分配节点内存
        node->key = malloc(strlen(keyArray[i]));
        memset(node->key, 0, strlen(keyArray[i] + 1));
        strcpy(node->key, keyArray[i]);

        node->value = malloc(strlen(valArray[i]));
        memset(node->value, 0, strlen(valArray[i] + 1));
        strcpy(node->value, valArray[i]);

        rbtree_insert(T, node);
    }

    rbtree_traversal(T, T->root);
    printf("----------------------------------------\n");

    for (i = 0; i < 10; i++)
    {
        rbtree_node *node = rbtree_search(T, keyArray[i]);
        rbtree_node *cur = rbtree_delete(T, node);

        free(cur->key);
        free(cur->value);
        free(cur);

        rbtree_traversal(T, T->root);
        printf("----------------------------------------\n");
    }

#else
    int keyArray[20] = {24, 25, 13, 35, 23, 26, 67, 47, 38, 98, 20, 19, 17, 49, 12, 21, 9, 18, 14, 15};
    rbtree *T = (rbtree *)malloc(sizeof(rbtree));
    if (T == NULL)
    {
        printf("malloc failed\n");
        return -1;
    }
    // 初始化nil节点
    T->nil = (rbtree_node *)malloc(sizeof(rbtree_node));
    T->nil->color = BLACK; // 默认nil的节点为黑色，红黑树性质3
    T->root = T->nil; // root默认指向nil，代表还没有结点
    rbtree_node *node = T->nil;
    int i = 0;
    for (i = 0; i < 20; i++)
    {
        node = (rbtree_node *)malloc(sizeof(rbtree_node)); // 分配节点内存
        node->key = keyArray[i];
        node->value = NULL;
        rbtree_insert(T, node);
    }
    rbtree_traversal(T, T->root);
    printf("----------------------------------------\n");
    for (i = 0; i < 20; i++)
    {
        rbtree_node *node = rbtree_search(T, keyArray[i]);
        rbtree_node *cur = rbtree_delete(T, node);
        free(cur);
        rbtree_traversal(T, T->root);
        printf("----------------------------------------\n");
    }
#endif

    return 0;
}
