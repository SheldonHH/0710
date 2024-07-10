#include "cxx_memory_relation/include/Memory.h"
#include <cstdio>

namespace org {
namespace memory {

void Memory::allocate_memory(size_t size) const {
    ptr = std::make_unique<uint8_t>(size);
    deallocate_thread = std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        deallocate_memory();
    });
    deallocate_thread.detach();
}

void Memory::deallocate_memory() const {
    ptr.reset();
}

const uint8_t* Memory::get_ptr() const {
    return ptr.get();
}

std::unique_ptr<Memory> new_memory() {
    return std::make_unique<Memory>();
}


}
}
