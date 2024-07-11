#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <vector>

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
    string mac_address;
    string client_uuid;

    Order(int id, const string& name, const string& type, double p, int vol, const string& mac, const string& client_uuid)
        : order_id(id), trader_name(name), order_type(type), price(p), volume(vol), mac_address(mac), client_uuid(client_uuid) {
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
                    {"price", price}, {"volume", volume}, {"timestamp", timestamp}, {"uuid", uuid}, 
                    {"mac_address", mac_address}, {"client_uuid", client_uuid}};
    }
};

void send_order(tcp::socket& socket, const Order& order) {
    json j = order.to_json();
    string message = j.dump() + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}

pair<string, string> generate_client_info() {
    // Generate UUID
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
    string client_uuid = uuid_ss.str();

    // Generate startup timestamp
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
    string startup_time = ss.str();

    return make_pair(client_uuid, startup_time);
}

void print_client_info(const string& client_uuid, const string& startup_time) {
    cout << "Client UUID: " << client_uuid << endl;
    cout << "Startup Time: " << startup_time << endl;
}

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

std::string get_mac_address() {
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);
    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS) {
        std::cerr << "GetAdaptersInfo failed: " << dwStatus << std::endl;
        return "";
    }

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    char mac_addr[18];
    sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
            pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2],
            pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
    return std::string(mac_addr);
}
#elif defined(__linux__)
#include <fstream>

std::string get_mac_address() {
    std::ifstream ifs("/sys/class/net/eth0/address");
    if (!ifs.is_open()) {
        std::cerr << "Failed to open MAC address file for eth0" << std::endl;
        return "";
    }
    std::string mac_address;
    std::getline(ifs, mac_address);
    return mac_address;
}
#else
std::string get_mac_address() {
    return "00:00:00:00:00:00"; // default MAC address for unsupported platforms
}
#endif

tcp::socket connect_to_server(boost::asio::io_context& io_context, const string& host, const string& port) {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    return socket;
}

Order create_order(int order_id, const string& mac_address, const string& client_uuid) {
    string trader_name = "Trader" + to_string(order_id);
    string order_type = (order_id % 2 == 0) ? "buy" : "sell";
    double price = 50000.0 + (order_id % 10) * 100;
    int volume = 1 + (order_id % 5);
    return Order(order_id, trader_name, order_type, price, volume, mac_address, client_uuid);
}

void process_orders(tcp::socket& socket, const string& mac_address, const string& client_uuid) {
    int order_id = 1;
    while (true) {
        Order order = create_order(order_id, mac_address, client_uuid);
        send_order(socket, order);
        order_id++;
        this_thread::sleep_for(chrono::milliseconds(5000)); // 模拟订单接收间隔
    }
}

int main() {
    try {
        // Generate and print client info
        auto [client_uuid, startup_time] = generate_client_info();
        print_client_info(client_uuid, startup_time);

        string mac_address = get_mac_address();
        cout << "MAC Address: " << mac_address << endl;

        const char* server_address = std::getenv("SERVER_ADDRESS");
        if (server_address == nullptr) {
            cerr << "SERVER_ADDRESS environment variable is not set" << endl;
            return 1;
        }

        string server_host = string(server_address).substr(0, string(server_address).find(":"));
        string server_port = string(server_address).substr(string(server_address).find(":") + 1);

        boost::asio::io_context io_context;
        tcp::socket socket = connect_to_server(io_context, server_host, server_port);

        process_orders(socket, mac_address, client_uuid);
    } catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
