#include <iostream>
#include <vector>
#include <future>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <stdexcept>
#include <type_traits>

using namespace std; // 使用 namespace std

// 线程池类，管理一组线程和任务队列
class ThreadPool {
public:
    // 构造函数，创建指定数量的线程
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        // 加锁，访问任务队列
                        unique_lock<mutex> lock(this->queueMutex);
                        // 等待任务或停止信号
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    // 向线程池中添加任务
    template <class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
        using returnType = typename result_of<F(Args...)>::type;

        auto task = make_shared<packaged_task<returnType()>>(
            bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        future<returnType> res = task->get_future();
        {
            unique_lock<mutex> lock(queueMutex);

            if (stop) {
                throw runtime_error("enqueue on stopped ThreadPool");
            }

            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    // 析构函数，停止所有线程
    ~ThreadPool() {
        {
            unique_lock<mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (thread &worker : workers) {
            worker.join();
        }
    }

private:
    vector<thread> workers; // 工作线程
    queue<function<void()>> tasks; // 任务队列

    mutex queueMutex; // 互斥锁
    condition_variable condition; // 条件变量
    bool stop; // 停止标志
};

// 实现 promisePool 函数
vector<future<void>> promisePool(const vector<function<void()>>& functions, size_t n) {
    ThreadPool pool(n); // 创建线程池
    vector<future<void>> results; // 存储任务结果的 future 对象
    mutex indexMutex; // 互斥锁，保护索引
    size_t index = 0; // 当前任务索引

    // 递归辅助函数
    function<void()> evaluateNext = [&]() {
        size_t currentIndex;
        {
            unique_lock<mutex> lock(indexMutex);
            if (index >= functions.size()) {
                return;
            }
            currentIndex = index++;
        }
        functions[currentIndex](); // 执行当前任务
        pool.enqueue(evaluateNext); // 执行下一个任务
    };

    // 启动前 n 个任务
    for (size_t i = 0; i < n && i < functions.size(); ++i) {
        results.push_back(pool.enqueue(evaluateNext));
    }

    return results;
}

// 示例主函数
int main() {
    // 定义一组任务
    vector<function<void()>> functions = {
        [] { cout << "Function 1\n"; },
        [] { cout << "Function 2\n"; },
        [] { cout << "Function 3\n"; },
        [] { cout << "Function 4\n"; },
        [] { cout << "Function 5\n"; }
    };

    // 使用 promisePool 执行任务，限制并发任务数为 2
    auto results = promisePool(functions, 2);
    // 等待所有任务完成
    for (auto& result : results) {
        result.get();
    }

    return 0;
}
