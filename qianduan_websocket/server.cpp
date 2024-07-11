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
#include <set>
#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using boost::asio::ip::tcp;
using json = nlohmann::json;
namespace websocket = boost::beast::websocket;

// Define Order structure
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
        // Generate timestamp
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        timestamp = ss.str();
    }

    // Convert Order to JSON
    json to_json() const {
        return json{{"order_id", order_id}, {"trader_name", trader_name}, {"order_type", order_type}, 
                    {"price", price}, {"volume", volume}, {"timestamp", timestamp}, {"uuid", uuid}, 
                    {"mac_address", mac_address}, {"client_uuid", client_uuid}};
    }

    // Convert JSON to Order
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

// ThreadPool class
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

// Exchange class
class Exchange {
public:
    Exchange(size_t num_threads) : thread_pool(num_threads) {}

    // Receive and process orders
    void receiveOrder(const Order& order, const string& client_ip) {
        thread_pool.enqueue(&Exchange::processOrder, this, order, client_ip);
    }

private:
    ThreadPool thread_pool;
    mutex order_book_mutex;
    vector<Order> buy_orders;
    vector<Order> sell_orders;

    // Match orders
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

                // Update order volume
                buy_orders.back().volume -= volume;
                sell_orders.back().volume -= volume;

                // Remove completed orders
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

    // Process orders
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

// WebSocket OrderHandler class
class WebSocketOrderHandler : public enable_shared_from_this<WebSocketOrderHandler> {
public:
    WebSocketOrderHandler(tcp::socket socket, Exchange& exchange)
        : ws_(move(socket)), exchange_(exchange) {}

    void run() {
        ws_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec) {
                if (!ec) {
                    addClient(self);
                    self->do_read();
                }
            });
    }

    // Send message to this client
    void send(json message) {
        ws_.async_write(boost::asio::buffer(message.dump()),
            [self = shared_from_this()](boost::system::error_code ec, size_t) {
                if (ec) {
                    std::cerr << "Send error: " << ec.message() << std::endl;
                }
            });
    }

    // Static method to send message to all clients
    static void broadcast(json message) {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        for (const auto& client : clients_) {
            client->send(message);
        }
    }

    // Static method to add a client
    static void addClient(std::shared_ptr<WebSocketOrderHandler> client) {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_.insert(client);
    }

    // Static method to remove a client
    static void removeClient(std::shared_ptr<WebSocketOrderHandler> client) {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_.erase(client);
    }

private:
    void do_read() {
        ws_.async_read(buffer_,
            [self = shared_from_this()](boost::system::error_code ec, size_t) {
                if (!ec) {
                    std::ostringstream ss;
                    ss << boost::beast::make_printable(self->buffer_.data());
                    auto msg = ss.str();

                    json j = json::parse(msg);
                    Order order = Order::from_json(j);
                    string client_ip = self->ws_.next_layer().remote_endpoint().address().to_string();
                    self->exchange_.receiveOrder(order, client_ip);

                    // Broadcast the order to all clients
                    broadcast(j);

                    self->buffer_.consume(self->buffer_.size());
                    self->do_read();
                } else {
                    removeClient(self);
                }
            });
    }

    websocket::stream<tcp::socket> ws_;
    boost::beast::flat_buffer buffer_;
    Exchange& exchange_;

    // Static members to keep track of all clients
    static std::set<std::shared_ptr<WebSocketOrderHandler>> clients_;
    static std::mutex clients_mutex_;
};

// Initialize static members
std::set<std::shared_ptr<WebSocketOrderHandler>> WebSocketOrderHandler::clients_;
std::mutex WebSocketOrderHandler::clients_mutex_;

// TCP OrderHandler class
class TCPOrderHandler : public enable_shared_from_this<TCPOrderHandler> {
public:
    TCPOrderHandler(tcp::socket socket, Exchange& exchange)
        : socket_(move(socket)), exchange_(exchange) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, buffer_, '\n',
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    std::istream is(&buffer_);
                    std::string msg;
                    std::getline(is, msg);

                    if (!msg.empty()) {
                        json j = json::parse(msg);
                        Order order = Order::from_json(j);
                        string client_ip = socket_.remote_endpoint().address().to_string();
                        exchange_.receiveOrder(order, client_ip);

                        // Broadcast the order to all WebSocket clients
                        WebSocketOrderHandler::broadcast(j);
                    }

                    do_read();
                }
            });
    }

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Exchange& exchange_;
};

// WebSocket Server class
class WebSocketServer {
public:
    WebSocketServer(boost::asio::io_context& io_context, short port, Exchange& exchange)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), exchange_(exchange) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    cout << "WebSocket connection accepted from " << socket.remote_endpoint() << endl;
                    auto handler = make_shared<WebSocketOrderHandler>(move(socket), exchange_);
                    WebSocketOrderHandler::addClient(handler);
                    handler->run();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
    Exchange& exchange_;
};

// TCP Server class
class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context, short port, Exchange& exchange)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), exchange_(exchange) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    cout << "TCP connection accepted from " << socket.remote_endpoint() << endl;
                    make_shared<TCPOrderHandler>(move(socket), exchange_)->start();
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

        Exchange exchange(4); // Create an exchange with 4 threads

        // WebSocket server on port 12346 for front-end
        WebSocketServer ws_server(io_context, 12346, exchange);

        // TCP server on port 12345 for C++ client
        TCPServer tcp_server(io_context, 12345, exchange);

        io_context.run();
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
