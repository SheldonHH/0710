#include <gtest/gtest.h>
#include <thread>

// 声明外部的 run_client 函数
extern void run_client(int order_id_base, int count);

// 测试用例
TEST(MultipleClientTest, SimulateClients) {
    const int num_clients = 5;  // 模拟 5 个客户端
    const int orders_per_client = 10;

    std::vector<std::thread> client_threads;

    // 启动多个客户端线程
    for (int i = 0; i < num_clients; ++i) {
        client_threads.emplace_back(run_client, i * orders_per_client + 1, orders_per_client);
    }

    // 等待所有客户端线程完成
    for (auto& t : client_threads) {
        t.join();
    }

    // 在这里可以添加对服务器状态的检查和验证
}

// 运行所有测试
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
