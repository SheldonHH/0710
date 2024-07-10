#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

// 线程池类
class ThreadPool {
public:
    // 构造函数，初始化线程池
    ThreadPool(size_t numThreads);

    // 提交任务到线程池
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    // 析构函数，关闭线程池
    ~ThreadPool();

private:
    // 工作线程函数，从任务队列中获取任务并执行
    void worker();

    // 线程池中的线程
    std::vector<std::thread> workers;

    // 任务队列
    std::queue<std::function<void()>> tasks;

    // 同步
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

// 实现构造函数
ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for(size_t i = 0; i < numThreads; ++i) {
        /* 请填充：将线程添加到workers向量中，并启动工作线程 */
        workers.emplace_back([this] { /* 请填充：调用worker函数 */ this->worker(); });
    }
}

// 实现任务提交函数
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using returnType = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<returnType> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        // 不允许在停止的线程池中添加任务
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        /* 请填充：将任务添加到任务队列中 */
        tasks.emplace([task](){ (*task)(); });
    }
    /* 请填充：通知一个等待中的线程 */
    condition.notify_one();
    return res;
}

// 实现析构函数
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    /* 请填充：通知所有等待中的线程 */
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

// 实现工作线程函数
void ThreadPool::worker() {
    while(true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(this->queueMutex);
            this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
            if(this->stop && this->tasks.empty())
                return;
            task = std::move(this->tasks.front());
            this->tasks.pop();
        }
        task();
    }
}

// 测试线程池
int main() {
    ThreadPool pool(4);

    auto result = pool.enqueue([](int answer) { return answer; }, 42);

    std::cout << "The answer is " << result.get() << std::endl;

    return 0;
}
