#include "Permutation_Rust_Call_CPP/include/Solution.h"
#include <vector>
#include <unordered_set>
#include <functional>
#include <sstream>

namespace org {
namespace solution {

std::vector<int> parse_input(const std::string& input) {
    std::vector<int> nums;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        nums.push_back(std::stoi(item));
    }
    return nums;
}

std::string serialize_output(const std::vector<std::vector<int>>& output) {
    std::stringstream ss;
    for (const auto& vec : output) {
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i != 0) ss << ",";
            ss << vec[i];
        }
        ss << ";";
    }
    std::string result = ss.str();
    if (!result.empty()) result.pop_back(); // 移除最后一个分号
    return result;
}

std::string Solution::permute(const std::string& nums_str) {
    std::vector<int> nums = parse_input(nums_str);
    std::vector<std::vector<int>> ans;
    int n = nums.size();
    std::vector<int> path(n, 0);
    std::unordered_set<int> s;

    std::function<void(int, std::unordered_set<int>)> dfs = [&] (int i, std::unordered_set<int> s) {
        if (i == n) {
            ans.emplace_back(path);
            return;
        }
        for (auto x: s) {
            path[i] = x;
            std::unordered_set<int> ns = s;
            ns.erase(x);
            dfs(i + 1, ns);
        }
    };

    dfs(0, std::unordered_set<int>(nums.begin(), nums.end()));
    return serialize_output(ans);
}

// 实现 new_solution 函数
std::unique_ptr<Solution> new_solution() {
    return std::make_unique<Solution>();
}

} // namespace solution
} // namespace org
