#pragma once
#include <memory>
#include <thread>
#include <chrono>

namespace org {
namespace memory {

class Memory {
public:
    void allocate_memory(size_t size);
    void deallocate_memory();
    const uint8_t* get_ptr() const;

private:
    std::unique_ptr<uint8_t[]> ptr;
    mutable std::thread deallocate_thread;
};

std::unique_ptr<Memory> new_memory();

}
}
