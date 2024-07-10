#include <iostream>
using namespace std;

class Solution {
public:
    void printFizz() {
        cout << "fizz ";
    }

    void printBuzz() {
        cout << "buzz ";
    }

    void printFizzBuzz() {
        cout << "fizzbuzz ";
    }

    void printNumber(int x) {
        cout << x << " ";
    }

    void fizzBuzz(int n) {
        for (int i = 1; i <= n; ++i) {
            if (i % 3 == 0 && i % 5 == 0) {
                printFizzBuzz();
            } else if (i % 3 == 0) {
                printFizz();
            } else if (i % 5 == 0) {
                printBuzz();
            } else {
                printNumber(i);
            }
        }
        cout << endl;
    }
};

int main() {
    int n = 15; // 设置最大值
    Solution solution;
    solution.fizzBuzz(n);

    return 0;
}
