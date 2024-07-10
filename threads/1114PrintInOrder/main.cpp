#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class Foo {
private:
    mutex mtx; // 互斥锁，用于保护共享变量order，保证同一时刻只有一个线程能够访问该变量
    condition_variable cv; // 条件变量，用于线程间同步，使线程可以等待某个条件成立后再继续执行
    int order; // 记录当前执行的顺序

public:
    Foo() : order(0) {
        
    }

    // first方法必须最先执行
    void first(function<void()> printFirst) {
        unique_lock<mutex> lock(mtx); // 获取互斥锁，保护共享变量
        // printFirst() 输出 "first"。不要改变或删除这行代码。
        printFirst();
        order = 1; // 更新执行顺序
        cv.notify_all(); // 通知所有等待的线程条件变量状态已改变
    }

    // second方法必须在first之后执行
    void second(function<void()> printSecond) {
        unique_lock<mutex> lock(mtx); // 获取互斥锁，保护共享变量
        cv.wait(lock, [this]() { return order == 1; }); // 等待条件变量，直到order为1
        // printSecond() 输出 "second"。不要改变或删除这行代码。
        printSecond();
        order = 2; // 更新执行顺序
        cv.notify_all(); // 通知所有等待的线程条件变量状态已改变
    }

    // third方法必须在second之后执行
    void third(function<void()> printThird) {
        unique_lock<mutex> lock(mtx); // 获取互斥锁，保护共享变量
        cv.wait(lock, [this]() { return order == 2; }); // 等待条件变量，直到order为2
        // printThird() 输出 "third"。不要改变或删除这行代码。
        printThird();
    }
};

// 定义输出函数
void printFirst() {
    cout << "first";
}

void printSecond() {
    cout << "second";
}

void printThird() {
    cout << "third";
}

int main() {
    Foo foo;

    thread t3(&Foo::third, &foo, printThird); // 创建线程t3执行third方法
    thread t1(&Foo::first, &foo, printFirst); // 创建线程t1执行first方法
    thread t2(&Foo::second, &foo, printSecond); // 创建线程t2执行second方法

    t3.join(); // 等待线程t3执行完毕
    t1.join(); // 等待线程t1执行完毕
    t2.join(); // 等待线程t2执行完毕

    cout << "所有线程处理完成" << endl;

    return 0;
}
