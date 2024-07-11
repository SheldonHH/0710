#include <iostream>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <cstring>

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

    Order(int id, const string& name, const string& type, double p, int vol, const string& mac, const string& client)
        : order_id(id), trader_name(name), order_type(type), price(p), volume(vol), mac_address(mac), client_uuid(client) {
        // Generate timestamp
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        timestamp = ss.str();

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
        uuid = uuid_ss.str();
    }

    json to_json() const {
        return json{{"order_id", order_id}, {"trader_name", trader_name}, {"order_type", order_type}, 
                    {"price", price}, {"volume", volume}, {"timestamp", timestamp}, 
                    {"uuid", uuid}, {"mac_address", mac_address}, {"client_uuid", client_uuid}};
    }
};

string get_mac_address() {
#if defined(__linux__) || defined(__APPLE__)
    array<char, 128> buffer;
    string result;
    shared_ptr<FILE> pipe(popen("ifconfig en0 | grep ether | awk '{print $2}'", "r"), pclose);
    if (!pipe) throw runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    result.erase(result.find_last_not_of(" \n\r\t") + 1);
    return result;
#elif defined(_WIN32)
    IP_ADAPTER_INFO AdapterInfo[16];
    DWORD dwBufLen = sizeof(AdapterInfo);
    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS) return "00:00:00:00:00:00";
    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    char mac_addr[18];
    sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
            pAdapterInfo->Address[0], pAdapterInfo->Address[1], pAdapterInfo->Address[2],
            pAdapterInfo->Address[3], pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
    return string(mac_addr);
#else
    return "00:00:00:00:00:00";
#endif
}

void send_order(tcp::socket& socket, const Order& order) {
    try {
        json j = order.to_json();
        string message = j.dump() + "\n";
        boost::asio::write(socket, boost::asio::buffer(message));
        cout << "Order sent: " << message << endl;
    } catch (const std::exception& e) {
        cerr << "Failed to send order: " << e.what() << endl;
    }
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

tcp::socket connect_to_server(boost::asio::io_context& io_context, const string& host, const string& port) {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    cout << "Connected to server: " << host << ":" << port << endl;
    return socket;
}

Order create_order(int order_id, const string& mac, const string& client_uuid) {
    string trader_name = "Trader" + to_string(order_id);
    string order_type = (order_id % 2 == 0) ? "buy" : "sell";
    double price = 50000.0 + (order_id % 10) * 100;
    int volume = 1 + (order_id % 5);
    return Order(order_id, trader_name, order_type, price, volume, mac, client_uuid);
}

void process_orders(tcp::socket& socket, const string& mac, const string& client_uuid) {
    int order_id = 1;
    while (true) {
        Order order = create_order(order_id, mac, client_uuid);
        send_order(socket, order);
        order_id++;
        this_thread::sleep_for(chrono::milliseconds(5000)); // Simulate order reception interval
    }
}

int main(int argc, char* argv[]) {
    try {
        // Generate and print client info
        auto [client_uuid, startup_time] = generate_client_info();
        print_client_info(client_uuid, startup_time);

        string mac_address = get_mac_address();
        cout << "MAC Address: " << mac_address << endl;

        string server_host;
        string server_port;

        if (argc > 1 && string(argv[1]) == "local") {
            server_host = "127.0.0.1";
            server_port = "12345";
        } else {
            const char* server_address = std::getenv("SERVER_ADDRESS");
            if (server_address == nullptr) {
                cerr << "SERVER_ADDRESS environment variable is not set" << endl;
                return 1;
            }

            server_host = string(server_address).substr(0, string(server_address).find(":"));
            server_port = string(server_address).substr(string(server_address).find(":") + 1);
        }

        boost::asio::io_context io_context;
        tcp::socket socket = connect_to_server(io_context, server_host, server_port);

        process_orders(socket, mac_address, client_uuid);
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
