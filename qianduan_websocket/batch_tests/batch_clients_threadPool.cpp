#include <iostream>
#include <stdexcept> // 使用invalid_argument
#include "ThreadPool.h"
#include "Tasks.h"

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

    // 创建线程池，线程数等于系统的并发线程数
    std::cout << "创建线程池，线程数等于系统的并发线程数: " << (std::thread::hardware_concurrency())<< std::endl;
    ThreadPool pool(std::thread::hardware_concurrency());

    // 提交任务到线程池
    for (int i = 0; i < num_times; ++i) {
        std::cout << "new taskss into threadPool !!!!! no: " << i  << std::endl;
        pool.enqueue(run_client, i + 1);
    }

    return 0;
}
