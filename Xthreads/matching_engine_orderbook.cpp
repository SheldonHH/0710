/*

g++ -std=c++11 matching_engine_orderbook.cpp -o matching_engine_orderbook -lpthread
# 这个错误表明在链接过程中缺少 pthread 库。pthread 是 POSIX 线程库，用于多线程编程。要解决这个问题，需要在编译时链接 pthread 库。
*/

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <chrono>
#include <map>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

using namespace std;

// 定义订单数据结构
struct Order {
    int order_id;
    string trader_name;
    string order_type; // buy or sell
    double price;
    int volume;
    string timestamp;
    string uuid;

    Order(int id, const string& name, const string& type, double p, int vol)
        : order_id(id), trader_name(name), order_type(type), price(p), volume(vol) {
        // 生成时间戳
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        timestamp = ss.str();

        // 生成UUID
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, 15);
        stringstream uuid_ss;
        uuid_ss << hex << uppercase;
        for (int i = 0; i < 8; i++) uuid_ss << dis(gen);
        uuid_ss << "-";
        for (int i = 0; i < 4; i++) uuid_ss << dis(gen);
        uuid_ss << "-4";
        for (int i = 0; i < 3; i++) uuid_ss << dis(gen);
        uuid_ss << "-";
        uuid_ss << dis(gen) << dis(gen) << dis(gen);
        uuid_ss << "-";
        for (int i = 0; i < 12; i++) uuid_ss << dis(gen);
        uuid = uuid_ss.str();
    }
};

// 线程池类
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex queue_mutex;
    condition_variable condition;
    bool stop;
};

ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                function<void()> task;

                {
                    unique_lock<mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;

    auto task = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));
    future<return_type> res = task->get_future();
    {
        unique_lock<mutex> lock(queue_mutex);

        if (stop)
            throw runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

ThreadPool::~ThreadPool() {
    {
        unique_lock<mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (thread &worker : workers)
        worker.join();
}

// 交易所类，处理订单
class Exchange {
public:
    Exchange(size_t num_threads) : thread_pool(num_threads) {}

    // 接收订单并将其提交到线程池处理
    void receiveOrder(Order order) {
        thread_pool.enqueue(&Exchange::processOrder, this, order);
    }

private:
    ThreadPool thread_pool;
    mutex order_book_mutex;
    vector<Order> buy_orders;
    vector<Order> sell_orders;

    // 撮合订单的方法
    void matchOrders() {
        lock_guard<mutex> lock(order_book_mutex);
        while (!buy_orders.empty() && !sell_orders.empty()) {
            auto buy_order = buy_orders.back();
            auto sell_order = sell_orders.back();

            if (buy_order.price >= sell_order.price) {
                int volume = min(buy_order.volume, sell_order.volume);
                cout << "Matched Order: Buy Order " << buy_order.order_id << " (" << buy_order.trader_name 
                     << ") and Sell Order " << sell_order.order_id << " (" << sell_order.trader_name 
                     << ") for volume " << volume << " at price " << sell_order.price << endl;

                // 更新订单量
                buy_orders.back().volume -= volume;
                sell_orders.back().volume -= volume;

                // 移除已完成的订单
                if (buy_orders.back().volume == 0) {
                    buy_orders.pop_back();
                }
                if (sell_orders.back().volume == 0) {
                    sell_orders.pop_back();
                }
            } else {
                break;
            }
        }
    }

    // 处理订单的方法
    void processOrder(Order order) {
        {
            lock_guard<mutex> lock(order_book_mutex);
            cout << "Received Order: " << order.order_id << " from " << order.trader_name 
                 << " (" << order.order_type << ") for volume " << order.volume << " at price " << order.price 
                 << " with timestamp " << order.timestamp << " and UUID " << order.uuid << endl;

            if (order.order_type == "buy") {
                buy_orders.push_back(order);
                sort(buy_orders.begin(), buy_orders.end(), [](const Order& a, const Order& b) {
                    return a.price > b.price;
                });
            } else if (order.order_type == "sell") {
                sell_orders.push_back(order);
                sort(sell_orders.begin(), sell_orders.end(), [](const Order& a, const Order& b) {
                    return a.price < b.price;
                });
            }
        }

        matchOrders();
    }
};

int main() {
    Exchange exchange(4); // 创建一个包含4个线程的交易所

    // 模拟接收订单
    vector<Order> orders = {
        Order(1, "Alice", "buy", 50000.0, 1),
        Order(2, "Bob", "sell", 50000.0, 2),
        Order(3, "Charlie", "buy", 50050.0, 1),
        Order(4, "David", "sell", 50000.0, 3),
        Order(5, "Eve", "buy", 50020.0, 1),
        Order(6, "Frank", "sell", 50020.0, 2),
    };

    for (const auto& order : orders) {
        exchange.receiveOrder(order);
        this_thread::sleep_for(chrono::milliseconds(50)); // 模拟订单接收间隔
    }

    this_thread::sleep_for(chrono::seconds(5)); // 等待所有订单处理完成

    return 0;
}
