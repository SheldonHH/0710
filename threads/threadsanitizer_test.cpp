#include <iostream>
#include <thread>
#include <vector>

volatile int shared_counter = 0;

void increment() {
    for (int i = 0; i < 1000; ++i) {
        ++shared_counter;
    }
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(increment));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter value: " << shared_counter << std::endl;
    return 0;
}
