#include <iostream>
using namespace std;

// 函数：返回 a 和 b 的最大公约数
// 使用递归方法实现
long long gcd(long long int a, long long int b)
{
    // 基本情况：如果 b 等于 0，返回 a
    // 因为任何数与 0 的最大公约数是这个数本身
    if (b == 0)
        return a;
    // 递归调用 gcd 函数，计算 b 和 a 对 b 取余后的结果的最大公约数
    // 这个步骤使用了欧几里得算法，逐步减少问题规模，最终返回最大公约数
    return gcd(b, a % b);
}

// 函数：返回两个数的最小公倍数
long long lcm(int a, int b)
{
    // 计算 a 和 b 的最小公倍数
    // 最小公倍数 (LCM) 可以通过以下公式计算：
    // LCM(a, b) = (a * b) / GCD(a, b)
    // 为了避免乘法溢出，改写为 (a / GCD(a, b)) * b
    long long result = (a / gcd(a, b)) * b;
    // 返回计算结果
    return result;
}

int main()
{
    int a = 24, b = 13; // 定义两个整数 a 和 b
    // 输出 a 和 b 的最小公倍数
    cout << "LCM : " << lcm(a, b);
    return 0; // 程序结束
}
