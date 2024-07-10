// CM-semaphore.cpp : Defines the entry point for the application.
//

#include "CM-semaphore.h"

using namespace std;

#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <chrono>
#include <functional>

// 创建一个计数信号量，初始值为3，表示最多允许3个线程同时访问资源
std::counting_semaphore<3> semaphore(3);

// 任务结构体，包含任务ID和任务函数
struct Task {
    int id;
    std::string description;
    std::function<void()> func;
};

// 线程函数，执行任务并访问共享资源
void executeTask(const Task& task) {
    // 请求信号量
    semaphore.acquire();

    // 模拟访问资源
    std::cout << "Thread " << task.id << " executing task: " << task.description << std::endl;
    task.func();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 释放信号量
    semaphore.release();

    std::cout << "Thread " << task.id << " finished task: " << task.description << std::endl;
}

int main() {
    const int numThreads = 10;
    std::vector<std::thread> threads;
    std::vector<Task> tasks;

    // 固定的任务描述数组
    std::string taskDescriptions[numThreads] = {
        "Task 1: Signal processing",
        "Task 2: Data encryption",
        "Task 3: Link maintenance",
        "Task 4: Satellite control",
        "Task 5: Telemetry analysis",
        "Task 6: Error correction",
        "Task 7: Frequency management",
        "Task 8: User data handling",
        "Task 9: Network monitoring",
        "Task 10: Ground station communication"
    };

    // 创建任务，每个任务有不同的操作和描述
    for (int i = 0; i < numThreads; ++i) {
        tasks.push_back({ i, taskDescriptions[i], [i, taskDescriptions]() {
            std::cout << "Performing " << taskDescriptions[i] << std::endl;
            // 这里可以添加具体的任务操作，比如模拟信号处理等
        } });
    }

    // 创建线程并执行不同的任务
    for (const auto& task : tasks) {
        threads.emplace_back(executeTask, std::ref(task));
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
