#include <iostream>
using namespace std;

// 字符串回文检查函数
bool isPalindrome(const string& S) {
    for (int i = 0; i < S.length() / 2; i++) {
        if (S[i] != S[S.length() - i - 1]) {
            return false;
        }
    }
    return true;
}

// 数字回文检查函数
bool isPalindrome(int n) {
    int original = n;
    int reversed = 0;

    while (n > 0) {
        int digit = n % 10;
        reversed = reversed * 10 + digit;
        n /= 10;
    }

    return original == reversed;
}

int main() {
    // 检查数字是否为回文
    int num;
    cout << "请输入一个数字：";
    cin >> num;
    if (isPalindrome(num)) {
        cout << num << " 是回文数。" << endl;
    } else {
        cout << num << " 不是回文数。" << endl;
    }

    // 检查字符串是否为回文
    string str;
    cout << "请输入一个字符串：";
    cin >> str;
    if (isPalindrome(str)) {
        cout << str << " 是回文字符串。" << endl;
    } else {
        cout << str << " 不是回文字符串。" << endl;
    }

    return 0;
}
