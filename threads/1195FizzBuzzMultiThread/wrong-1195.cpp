#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;

class FizzBuzz {
private:
    int n; // 最大值
    atomic<int> current; // 当前处理的数字，用原子变量以避免竞态条件
    mutex mtx; // 互斥锁保护共享资源
    condition_variable cv; // 条件变量用于线程同步

public:
    FizzBuzz(int n) {
        this->n = n;
        current = 1;
    }

    // 输出 "fizz"
    void fizz(function<void()> printFizz) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return (current > n || (current % 3 == 0 && current % 5 != 0 && current % 7 == 0)); });
            if (current > n) break; // 超出范围时退出
            if (current % 3 == 0 && current % 5 != 0 && current % 7 == 0) {
                cout << "@@@@ fizz 条件触发 @@@@" << endl;
                printFizz();
                current++;
                cout << "线程 fizz 处理数字: " << current - 1 << endl;
                cv.notify_all(); // 通知其他线程
            }
        }
    }

    // 输出 "buzz"
    void buzz(function<void()> printBuzz) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return (current > n || (current % 5 == 0 && current % 3 != 0 && current % 7 == 0)); });
            if (current > n) break; // 超出范围时退出
            if (current % 5 == 0 && current % 3 != 0 && current % 7 == 0) {
                cout << "@@@@ buzz 条件触发 @@@@" << endl;
                printBuzz();
                current++;
                cout << "线程 buzz 处理数字: " << current - 1 << endl;
                cv.notify_all(); // 通知其他线程
            }
        }
    }

    // 输出 "fizzbuzz"
    void fizzbuzz(function<void()> printFizzBuzz) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return (current > n || (current % 3 == 0 && current % 5 == 0 && current % 7 == 0)); });
            if (current > n) break; // 超出范围时退出
            if (current % 3 == 0 && current % 5 == 0 && current % 7 == 0) {
                cout << "@@@@ fizzbuzz 条件触发 @@@@" << endl;
                printFizzBuzz();
                current++;
                cout << "线程 fizzbuzz 处理数字: " << current - 1 << endl;
                cv.notify_all(); // 通知其他线程
            }
        }
    }

    // 输出数字
    void number(function<void(int)> printNumber) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [this]() { return (current > n || (current % 3 != 0 && current % 5 != 0 && current % 7 != 0)); });
            if (current > n) break; // 超出范围时退出
            if (current % 3 != 0 && current % 5 != 0 && current % 7 != 0) {
                cout << "@@@@ number 条件触发 @@@@" << endl;
                printNumber(current);
                current++;
                cout << "线程 number 处理数字: " << current - 1 << endl;
                cv.notify_all(); // 通知其他线程
            }
        }
    }
};

// 定义输出函数
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

int main() {
    int n = 105; // 设置最大值
    FizzBuzz fizzBuzz(n);

    // 创建并启动线程
    thread t1(&FizzBuzz::fizz, &fizzBuzz, printFizz);
    thread t2(&FizzBuzz::buzz, &fizzBuzz, printBuzz);
    thread t3(&FizzBuzz::fizzbuzz, &fizzBuzz, printFizzBuzz);
    thread t4(&FizzBuzz::number, &fizzBuzz, printNumber);

    // 等待所有线程完成
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "所有线程处理完成" << endl;

    return 0;
}
