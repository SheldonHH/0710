#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool number_turn = true;

void print_numbers() {
    /* 填充此处 */
}

void print_letters() {
    /* 填充此处 */
}

int main() {
    std::thread t1(print_numbers);
    std::thread t2(print_letters);

    t1.join();
    t2.join();

    return 0;
}
