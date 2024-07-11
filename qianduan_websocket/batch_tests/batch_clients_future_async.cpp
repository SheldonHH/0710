#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <future>

void run_client(int id, std::mutex& mtx) {
    std::string cmd = "./client local";
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread " << id << " executing: " << cmd << std::endl;
    }
    int ret = system(cmd.c_str());
    if (ret != 0) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cerr << "Thread " << id << " command failed with code " << ret << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_times>" << std::endl;
        return 1;
    }

    int num_times;
    try {
        num_times = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number: " << argv[1] << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Number out of range: " << argv[1] << std::endl;
        return 1;
    }

    // 创建线程池
    std::vector<std::future<void>> futures;
    std::mutex mtx;

    for (int i = 0; i < num_times; ++i) {
        futures.emplace_back(std::async(std::launch::async, run_client, i + 1, std::ref(mtx)));
    }

    // 等待所有任务完成
    for (auto& fut : futures) {
        fut.get();
    }

    return 0;
}
