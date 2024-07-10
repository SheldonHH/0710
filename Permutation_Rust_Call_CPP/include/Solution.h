#include <string>
#include <memory>
#include <unordered_set>

namespace org {
namespace solution {

class Solution {
public:
    std::string permute(const std::string& nums_str);
};

// 定义 new_solution 函数声明
std::unique_ptr<Solution> new_solution();

} // namespace solution
} // namespace org

