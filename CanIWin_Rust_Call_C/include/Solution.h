#include <unordered_map>
#include <functional>
#include <memory>

namespace org {
namespace solution {

class Solution {
public:
    bool canIWin(int maxChoosableInteger, int desiredTotal) const;
};

// 定义 new_solution 函数声明
std::unique_ptr<Solution> new_solution();

}
}
