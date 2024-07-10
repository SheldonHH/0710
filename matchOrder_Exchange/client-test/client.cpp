#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

using namespace std;
using boost::asio::ip::tcp;
using json = nlohmann::json;

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

    json to_json() const {
        return json{{"order_id", order_id}, {"trader_name", trader_name}, {"order_type", order_type}, 
                    {"price", price}, {"volume", volume}, {"timestamp", timestamp}, {"uuid", uuid}};
    }
};

void send_order(tcp::socket& socket, const Order& order) {
    json j = order.to_json();
    string message = j.dump() + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}

// 添加run_client函数
void run_client(int num_orders, int delay) {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "12345");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        int order_id = 1;
        for (int i = 0; i < num_orders; ++i) {
            string trader_name = "Trader" + to_string(order_id);
            string order_type = (order_id % 2 == 0) ? "buy" : "sell";
            double price = 50000.0 + (order_id % 10) * 100;
            int volume = 1 + (order_id % 5);
            Order order(order_id, trader_name, order_type, price, volume);

            send_order(socket, order);

            order_id++;
            this_thread::sleep_for(chrono::milliseconds(delay)); // 使用参数指定的延迟时间
        }
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
}

// 主函数，可以保留也可以移除
// int main() {
//     run_client(10, 5000); // 示例调用，可以根据需要调整
//     return 0;
// }