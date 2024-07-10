#include <iostream>
#include <vector>
#include <thread>

void sum() {
    // 一些需要在线程中执行的任务
    std::cout << "Thread is running." << std::endl;
}

int main() {
    std::vector<std::thread> mybox;
    for (size_t i = 0; i < 10; ++i) {
        mybox.emplace_back(sum);
    }

// for (std::thread& t : mybox)：

// 遍历 mybox 中的每个 std::thread 对象，使用引用避免复制。
// 不能使用 const std::thread&，因为 t.join() 需要修改线程对象的状态
    for (std::thread& t : mybox) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
