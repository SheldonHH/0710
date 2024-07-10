#include "Permutation_Rust_Call_CPP/include/Solution.h"
#include <vector>
#include <unordered_set>
#include <functional>

namespace org {
namespace solution {

std::vector<std::vector<int>> Solution::permute(std::vector<int>& nums) {
    std::vector<std::vector<int>> ans;
    int n = nums.size();
    std::vector<int> path(n, 0);
    std::unordered_set<int> s;

    // 通过这些操作，DFS函数能够生成所有可能的排列组合，并将它们存储在 ans 中返回。
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
    return ans;
}

// 实现 new_solution 函数
std::unique_ptr<Solution> new_solution() {
    return std::make_unique<Solution>();
}

} // namespace solution
} // namespace org
