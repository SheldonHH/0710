#include <iostream>
#include <thread>
#include <chrono>
#include "rust/cxx.h"
#include "Memory.rs.h"

int main() {
    size_t size = 32;

    rust::Box<Memory> mem = new_memory();

    std::cout << "========================   C++   ========================" << std::endl;
    std::cout << "- Pointer before memory allocation: " << static_cast<const void*>(mem->get_ptr()) << std::endl;
    mem->allocate_memory(size);
    const uint8_t* ptr = mem->get_ptr();
    std::cout << "- Allocated memory at: " << static_cast<const void*>(ptr) << ", value is " << static_cast<int>(*ptr) << std::endl;

    // Wait for 2 seconds for the memory at pointer to be deallocated on the Rust side
    std::cout << "Deallocating memory" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "- Checking value (" << static_cast<int>(*ptr) << ") of pointer: " << static_cast<const void*>(ptr) << std::endl;
    std::cout << "=========================================================" << std::endl;

    std::cout << "\n========================   Rust   ========================" << std::endl;
    uint32_t* ptr_u32 = (uint32_t*)ptr;
    std::cout << "- Data at pointer (" << static_cast<const void*>(ptr_u32) << "): " << *ptr_u32 << std::endl;

    uint32_t n = 12;
    std::cout << "Writing '" << n << "' to memory" << std::endl;
    *ptr_u32 = n;
    std::cout << "- Checking value at pointer (" << static_cast<const void*>(ptr_u32) << "): " << *ptr_u32 << std::endl;
    std::cout << "=========================================================" << std::endl;

    return 0;
}
