#include <stdio.h>
#include <stdlib.h>

/**
 * 链表节点结构体
 */
typedef struct ListNode
{
    int val;                // 节点值
    struct ListNode* next;  // 指向下一节点的指针
} ListNode;

/**
 * 链表对应的构造函数
 */
ListNode* newNode(int val)
{
    ListNode* node;
    node = (ListNode*)malloc(sizeof(ListNode));
    node->val = val;
    node->next = NULL;

    return node;
}


/**
 * 插入节点
 * 
 * 假设我们想在相邻的两个节点 n0 和 n1 之间插入一个新节点 P ，
 * 则只需改变两个节点引用（指针）即可，时间复杂度为 O(1)。
 * 
 * 在链表的节点 n0 之后插入节点 P
 */
void insertNode(ListNode* n0, ListNode* P)
{
    ListNode* n1 = n0->next; // 获得 n0 的下一个节点 n1
    P->next = n1; // 先使 P 指向 n1
    n0->next = P; // 再使 P 的前一个节点(n0)指向 P
}


/**
 * 删除节点(改变目标节点前一个节点的指向即可。)
 * 
 * 删除链表的节点 n0 之后的首个节点。
 */
void deleteNode(ListNode* n0)
{
    if (!n0->next)
        return;
    
    // n0 -> P -> n1
    ListNode* P = n0->next;
    ListNode* n1 = P->next;
    n0->next = n1;

    // 释放内存
    free(P);
}


/**
 * 访问节点
 * 
 * 在链表中，程序需要从头节点出发，逐个向后遍历，直至找到目标节点。时间复杂度为 O(n)。
 * 
 * 访问链表中索引为 index 的节点:
 */
ListNode* accessNode(ListNode* head, int index)
{
    for (int i = 0; i < index; i++)
    {
        if (head == NULL) return NULL;
        head = head->next;
    }

    return head;
}


/**
 * 查找节点
 * 
 * 返回目标节点的索引值, 头节点索引值为0
 */
int findNode(ListNode* head, int target)
{
    int index = 0;

    while (head)
    {
       if (head->val == target) return index;

       head = head->next;
       index++;
    }
    
    return -1;
}


/**
 * 释放内存
 * 
 */
void freeNode(ListNode* head)
{
    ListNode* currNode = head;

    while (currNode)
    {
        ListNode* next = currNode->next; // 保存下一个节点
        free(currNode); // 释放当前节点
        currNode = next; // 移动到下一个节点
    }
}


int main(void)
{
    /**
     * 链表的初始化
     * 
     * 建立链表分为两步，第一步是初始化各个节点对象，第二步是构建节点之间的引用关系。
     * 初始化完成后，我们就可以从链表的头节点出发，通过引用指向 next 依次访问所有节点。
     * 
     * 我们通常将头节点当作链表的代称，比如以下代码中的链表可记作链表 n0
     */
    // 初始化新建节点的值
    ListNode* n0 = newNode(1);
    ListNode* n1 = newNode(3);
    ListNode* n2 = newNode(2);
    ListNode* n3 = newNode(5);
    ListNode* n4 = newNode(4);
    // 构建节点间的相互引用
    n0->next = n1;
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;

    /**
     * 插入节点示例
     */
    ListNode* P = newNode(99);

    printf("(Bf insert) Value of n2 next node: %d\n", n2->next->val); // 5
    insertNode(n2, P);
    printf("(Af insert) Value of n2 next node: %d\n\n", n2->next->val); // 99

    /**
     * 删除节点示例
     */
    printf("(Bf delete) Value of n2 next node: %d\n", n2->next->val); // 99
    deleteNode(n2); // 删除 n2 节点后的首个节点
    printf("(Af delete) Value of n2 next node: %d\n\n", n2->next->val); // 5

    /**
     * 访问节点示例
     * 
     * 访问链表中索引为 index 的节点(此时头节点索引为0):
     */
    int index_access = 2;
    ListNode* node = accessNode(n0, index_access);
    printf("The value of the node when index=%d: %d\n\n", index_access, node->val);

    /**
     * 查找节点示例
     */
    int target = 5;
    int index_find = findNode(n0, target);
    printf("The index of the node whose value is %d: %d\n\n", target, index_find);

    /**
     * 释放链表空间示例
     */
    freeNode(n0);

    return 0;
}
