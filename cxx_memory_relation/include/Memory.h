#include <memory>
#include <thread>
#include <chrono>

namespace org {
namespace memory {

struct Memory {
public:
    mutable std::unique_ptr<uint8_t> ptr;

    Memory() { ptr = nullptr; }

    void allocate_memory(size_t size) const;
    void deallocate_memory() const;
    const uint8_t* get_ptr() const;

private:
    mutable std::thread deallocate_thread;
};

std::unique_ptr<Memory> new_memory();

}
}
