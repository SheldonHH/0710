#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool number_turn = true;

void print_numbers() {
    for (int i = 1; i <= 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return number_turn; });
        std::cout << i;
        number_turn = false;
        cv.notify_all();
    }
}

void print_letters() {
    for (char c = 'A'; c <= 'E'; ++c) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !number_turn; });
        std::cout << c;
        number_turn = true;
        cv.notify_all();
    }
}

int main() {
    std::thread t1(print_numbers);
    std::thread t2(print_letters);

    t1.join();
    t2.join();

    return 0;
}
