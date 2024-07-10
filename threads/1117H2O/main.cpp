// 于控制多个线程按正确的顺序输出“H2O”，即保证两个氢原子和一个氧原子形成一个水分子。具体来说，需要设计一种机制，使得每个线程调用 hydrogen 或 oxygen 方法时能够按照H2O的顺序输出字符。
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class H2O {
private:
    int hydrogen_count; // 记录氢原子数量
    int oxygen_count; // 记录氧原子数量
    mutex mtx; // 互斥锁
    condition_variable cv; // 条件变量

public:
    H2O() : hydrogen_count(0), oxygen_count(0) {}

    void hydrogen(function<void()> releaseHydrogen) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return hydrogen_count < 2; }); // 等待条件变量，直到可以输出氢原子
        // releaseHydrogen() 输出 "H"。不要改变或删除这行代码。
        releaseHydrogen();
        hydrogen_count++; // 增加氢原子计数
        if (hydrogen_count == 2 && oxygen_count == 1) { // 如果形成一个水分子
            hydrogen_count = 0;
            oxygen_count = 0;
        }
        cv.notify_all(); // 通知所有等待的线程条件变量状态已改变
    }

    void oxygen(function<void()> releaseOxygen) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this]() { return oxygen_count < 1; }); // 等待条件变量，直到可以输出氧原子
        // releaseOxygen() 输出 "O"。不要改变或删除这行代码。
        releaseOxygen();
        oxygen_count++; // 增加氧原子计数
        if (hydrogen_count == 2 && oxygen_count == 1) { // 如果形成一个水分子
            hydrogen_count = 0;
            oxygen_count = 0;
        }
        cv.notify_all(); // 通知所有等待的线程条件变量状态已改变
    }
};
void releaseHydrogen() {
    cout << "H";
}

void releaseOxygen() {
    cout << "O";
}
int main() {
    H2O h2o;

    // 创建多个线程来调用 hydrogen 和 oxygen 方法
    thread threads[6];

    // 创建两个线程调用 hydrogen
    threads[0] = thread(&H2O::hydrogen, &h2o, releaseHydrogen);
    threads[1] = thread(&H2O::hydrogen, &h2o, releaseHydrogen);

    // 创建一个线程调用 oxygen
    threads[2] = thread(&H2O::oxygen, &h2o, releaseOxygen);

    // 创建两个线程调用 hydrogen
    threads[3] = thread(&H2O::hydrogen, &h2o, releaseHydrogen);
    threads[4] = thread(&H2O::hydrogen, &h2o, releaseHydrogen);

    // 创建一个线程调用 oxygen
    threads[5] = thread(&H2O::oxygen, &h2o, releaseOxygen);

    // 等待所有线程完成
    for (int i = 0; i < 6; ++i) {
        threads[i].join();
    }

    cout << endl << "所有线程处理完成" << endl;

    return 0;
}
