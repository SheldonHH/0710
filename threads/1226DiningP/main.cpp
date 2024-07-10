
// LeetCode 1226 题目是关于著名的哲学家就餐问题，需要实现一个解决方案，以确保哲学家们不会发生死锁。哲学家们围坐在圆桌旁，每人两边各有一根叉子，只有同时拿到两根叉子才能吃饭。

// 实现 DiningPhilosophers 类，确保哲学家在拿起和放下叉子时不会造成死锁，并且不会饿死任何哲学家。以下是一个基于标准 mutex 和 condition_variable 的解决方案。

#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

class DiningPhilosophers {
private:
    mutex mtx;
    vector<mutex> forks; // 每个叉子对应一个互斥锁

public:
    DiningPhilosophers() : forks(5) {} // 初始化5个叉子

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        int leftFork = philosopher;
        int rightFork = (philosopher + 1) % 5;

        unique_lock<mutex> lock(mtx); // 全局互斥锁，确保死锁不会发生

        // 哲学家首先拿起编号较小的叉子，再拿起编号较大的叉子
        lock_guard<mutex> lockLeftFork(forks[min(leftFork, rightFork)]);
        lock_guard<mutex> lockRightFork(forks[max(leftFork, rightFork)]);

        // 拿起左叉子
        pickLeftFork();
        // 拿起右叉子
        pickRightFork();
        // 吃饭
        eat();
        // 放下左叉子
        putLeftFork();
        // 放下右叉子
        putRightFork();
    }
};

// 测试函数
void testDiningPhilosophers() {
    DiningPhilosophers philosophers;

    vector<thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.push_back(thread(&DiningPhilosophers::wantsToEat, &philosophers, i,
            []() { cout << "P" << this_thread::get_id() << " picks up left fork\n"; },
            []() { cout << "P" << this_thread::get_id() << " picks up right fork\n"; },
            []() { cout << "P" << this_thread::get_id() << " is eating\n"; },
            []() { cout << "P" << this_thread::get_id() << " puts down left fork\n"; },
            []() { cout << "P" << this_thread::get_id() << " puts down right fork\n"; }
        ));
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    testDiningPhilosophers();
    cout << "所有哲学家处理完成" << endl;
    return 0;
}
