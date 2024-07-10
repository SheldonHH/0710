#include <iostream>
using namespace std;

// 函数：返回 a 和 b 的最大公约数
int gcd(int a, int b)
{
    // 初始化 result 为 a 和 b 中较小的那个数
    // 因为一个数的最大公约数不会大于这个数本身
    int result = min(a, b); 

    // 循环查找最大公约数
    while (result > 0) {
        // 检查 result 是否同时是 a 和 b 的约数
        // 如果 a 和 b 都能被 result 整除，那么 result 就是它们的一个公约数
        if (a % result == 0 && b % result == 0) {
            // 找到最大公约数，退出循环
            break; 
        }
        // 将 result 减 1，继续查找
        // 逐一减少 result，确保找到的第一个符合条件的 result 就是最大公约数
        result--; 
    }

    // 返回最大公约数
    return result; 
}

int main()
{
    int a = 54, b = 33; // 定义两个整数 a 和 b

    // 输出 a 和 b 的最大公约数
    cout << "GCD: " << gcd(a, b); 

    return 0; // 程序结束
}
