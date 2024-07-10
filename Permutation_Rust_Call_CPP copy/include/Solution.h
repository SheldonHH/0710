#include <vector>
#include <memory>
#include <unordered_set>
#include <functional>

namespace org {
namespace solution {

class Solution {
public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums);
};

// 定义 new_solution 函数声明
std::unique_ptr<Solution> new_solution();

} // namespace solution
} // namespace org
