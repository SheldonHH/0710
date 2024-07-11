#include <iostream>
#include <cstdlib>  // 使用system函数
#include <sstream>  // 使用ostringstream
#include <string>   // 使用string
#include <stdexcept> // 使用invalid_argument
#include <cctype>   // 使用isdigit

int main(int argc, char* argv[]) {
    // 检查参数个数
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_times>" << std::endl;
        return 1;
    }

    // 将参数转换为整数
    int num_times;
    try {
        // 检查参数是否为数字
        std::string arg = argv[1];
        if (!std::all_of(arg.begin(), arg.end(), ::isdigit)) {
            throw std::invalid_argument("Argument is not a number.");
        }

        // 转换参数为整数
        num_times = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number: " << argv[1] << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Number out of range: " << argv[1] << std::endl;
        return 1;
    }

    // 执行指定次数的 ./client local 命令
    for (int i = 0; i < num_times; ++i) {
        std::string cmd = "./client local";
        std::cout << "Executing: " << cmd << " (" << i + 1 << "/" << num_times << ")" << std::endl;
        int ret = system(cmd.c_str());
        if (ret != 0) {
            std::cerr << "Command failed with code " << ret << std::endl;
            return ret;
        }
    }

    return 0;
}
