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
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using boost::asio::ip::tcp;
using json = nlohmann::json;

// 定义订单数据结构
struct Order {
    int order_id;
    string trader_name;
    string order_type; // buy or sell
    double price;
    int volume;
    string timestamp;
    string uuid;
    string mac_address;
    string client_uuid;

    Order() = default;

    Order(int id, const string& name, const string& type, double p, int vol, const string& uuid, const string& mac, const string& client_uuid)
        : order_id(id), trader_name(name), order_type(type), price(p), volume(vol), uuid(uuid), mac_address(mac), client_uuid(client_uuid) {
        // 生成时间戳
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        timestamp = ss.str();
    }

    // 将订单转换为JSON
    json to_json() const {
        return json{{"order_id", order_id}, {"trader_name", trader_name}, {"order_type", order_type}, 
                    {"price", price}, {"volume", volume}, {"timestamp", timestamp}, {"uuid", uuid}, 
                    {"mac_address", mac_address}, {"client_uuid", client_uuid}};
    }

    // 从JSON转换为订单
    static Order from_json(const json& j) {
        Order order;
        order.order_id = j.at("order_id").get<int>();
        order.trader_name = j.at("trader_name").get<string>();
        order.order_type = j.at("order_type").get<string>();
        order.price = j.at("price").get<double>();
        order.volume = j.at("volume").get<int>();
        order.timestamp = j.at("timestamp").get<string>();
        order.uuid = j.at("uuid").get<string>();
        order.mac_address = j.at("mac_address").get<string>();
        order.client_uuid = j.at("client_uuid").get<string>();
        return order;
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
    void receiveOrder(const Order& order, const string& client_ip) {
        thread_pool.enqueue(&Exchange::processOrder, this, order, client_ip);
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
    void processOrder(Order order, const string& client_ip) {
        {
            lock_guard<mutex> lock(order_book_mutex);
            cout << "Received Order: " << order.order_id << " from " << order.trader_name 
                 << " (" << order.order_type << ") for volume " << order.volume << " at price " << order.price 
                 << " with timestamp " << order.timestamp 
                 << ", Client UUID: " << order.client_uuid.substr(order.client_uuid.size() - 5)
                 << ", Client MAC: " << order.mac_address
                 << ", Client IP: " << client_ip << endl;

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

// 订单处理类，处理单个客户端的订单请求
class OrderHandler : public enable_shared_from_this<OrderHandler> {
public:
    OrderHandler(tcp::socket socket, Exchange& exchange)
        : socket_(move(socket)), exchange_(exchange) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, boost::asio::dynamic_buffer(data_), '\n',
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    stringstream ss(data_.substr(0, length));
                    json j;
                    ss >> j;
                    Order order = Order::from_json(j);
                    string client_ip = socket_.remote_endpoint().address().to_string();
                    exchange_.receiveOrder(order, client_ip);
                    data_.erase(0, length);
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    string data_;
    Exchange& exchange_;
};

// 服务器类，监听并处理客户端请求
class Server {
public:
    Server(boost::asio::io_context& io_context, short port, Exchange& exchange)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), exchange_(exchange) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    cout << "Accepted connection from " << socket.remote_endpoint() << endl;
                    shared_ptr<OrderHandler> handler = make_shared<OrderHandler>(move(socket), exchange_);
                    handler->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
    Exchange& exchange_;
};

int main() {
    std::cout << "Order Matching Server started..." << std::endl;
    try {
        boost::asio::io_context io_context;

        Exchange exchange(4); // 创建一个包含4个线程的交易所

        Server server(io_context, 12345, exchange); // 创建服务器，监听端口12345

        io_context.run();
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
