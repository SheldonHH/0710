#include <iostream>
#include <vector>
#include "success-CPP-call-Rust-permutation/include/rust_functions.h"
int main() {
    std::vector<int32_t> nums = {1, 2, 3};
    auto result = permute(nums);

    for (const auto& vec : result) {
        for (int num : vec) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
