#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <cstdlib>  // 使用system函数
#include <string>   // 使用string

void run_client(int id) {
    std::string cmd = "./client local";
    std::cout << "Thread " << id << " executing: " << cmd << std::endl;
    int ret = system(cmd.c_str());
    if (ret != 0) {
        std::cerr << "Thread " << id << " command failed with code " << ret << std::endl;
    }
}

#endif // TASK_H
