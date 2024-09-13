#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 双层 for 循环
 * 
 * 在这种情况下，函数的操作数量与 n^2 成正比，或者说算法运行时间和输入数据大小 n 成“平方关系”。
 * 我们可以继续添加嵌套循环，每一次嵌套都是一次“升维”，将会使时间复杂度提高至“立方关系”“四次方关系”，以此类推。
 * 
 */
char* nestedForLoop(int n)
{
    int size = n * n * 26 + 1;
    char* res = malloc(size * sizeof(char));
    // 外部循环
    for(int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= 5; j++)
        {
            char tmp[26];
            snprintf(tmp, sizeof(tmp), "(%d, %d), ", i, j);
            strncat(res, tmp, size - strlen(res) - 1);
        }
    }

    return res;
}


/**
 * 递归
 * 
 * 递归(recursion)是一种算法策略，函数通过调用自身来解决问题。主要包含两个阶段:
 * 1. 递：程序不断深入地调用自身，通常传入更小或更简化的参数，直到达到“终止条件”。
 * 2. 归：触发“终止条件”后，程序从最深层的递归函数开始逐层返回，汇聚每一层的结果。
 * 
 * 从实现的角度看，递归代码主要包含三个要素:
 * 1. 终止条件
 * 2. 递归调用，对应“递”，函数调用自身，通常输入更小或更简化的参数。
 * 3. 返回结果, 对应“归”，将当前递归层级的结果返回至上一层。
 * 
 */
int recursion(int n)
{
    // 终止条件
    if (n == 1)
        return 1;
    
    // 递: 递归调用
    int res = recursion(n - 1);

    // 归: 返回结果
    return n + res;
}


/**
 * 尾递归
 * 
 * 如果函数在返回前的最后一步才进行递归调用，则该函数可以被编译器或解释器优化，
 * 使其在空间效率上与迭代相当。这种情况被称为尾递归（tail recursion）。
 * 
 * 普通递归：当函数返回到上一层级的函数后，需要继续执行代码，因此系统需要保存上一层调用的上下文。
 * 尾递归：递归调用是函数返回前的最后一个操作，这意味着函数返回到上一层级后，无须继续执行其他操作，因此系统无须保存上一层函数的上下文。
 * 
 */
int tailRecursion(int n, int res)
{
    // 终止条件
    if (n == 0)
        return res;

    // 尾递归调用
    return tailRecursion(n - 1, res + n);
}


/**
 * 递归求解斐波那契数列的第 n 个数的值
 * 
 * 观察以上代码，我们在函数内递归调用了两次函数，这意味着从一个调用产生了两个调用分支。
 * 这样不断递归调用下去，最终将产生一棵层数为 n 的递归树（recursion tree）。
 */
int fib(int n)
{
    // 终止条件
    if (n == 1 || n == 2)
        return n - 1;
    
    // 递归调用
    int res = fib(n - 1) + fib(n -2);

    // 返回结果
    return res;
}


/**
 * 使用迭代模拟递归
 * 
 * 1. 递：当函数被调用时，系统会在“调用栈”上为该函数分配新的栈帧，用于存储函数的局部变量、参数、返回地址等数据。
 * 2. 归：当函数完成执行并返回时，对应的栈帧会被从“调用栈”上移除，恢复之前函数的执行环境。
 * 
 */
int forLoopRecursion(int n)
{
    // 使用一个大数组模拟stack
    int stack[1000];
    int top = -1; // 栈顶索引
    int res = 0;
    // 递: 递归调用
    for (int i = n; i > 0; i--)
    {
        // 通过“入栈操作”模拟“递”
        /*
        stack[1 + top++] = i; 这一行的操作可以分解为两步：
        1.	访问 stack[1 + top] 并赋值 i。
        2.	将 top 自增。
        */
        stack[1 + top++] = i;
    }
    // 归: 返回结果
    while (top >= 0)
    {
        // 通过“出栈操作”模拟“归”
        res += stack[top--];
    }

    // res = 1 + 2 + 3 + ... + n
    return res;
}

int main(void)
{
    /**
     * 嵌套循环测试
     */
    int n = 5;
    char* res = nestedForLoop(n);
    printf("双层 for 循环的遍历结果: %s\n", res);


    /**
     * 递归测试
     */
    int res2 = recursion(5);
    printf("递归函数求和结果: %d\n", res2);

    /**
     * 尾递归测试
     */
    int res3 = tailRecursion(5, 0);
    printf("尾递归函数求和结果: %d\n", res3);

    /**
     * 斐波那契求值函数测试
     */
    int index = 8;
    int res4 = fib(index);
    printf("斐波那契数列第 %d 个数的值为: %d\n", index, res4);

    /**
     * 使用迭代模拟递归测试
     */
    int res5 = forLoopRecursion(5);
    printf("使用迭代模拟递归计算结果: %d\n", res5);


    return 0;
}