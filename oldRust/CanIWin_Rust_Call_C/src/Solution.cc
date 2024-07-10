#include "CanIWin_Rust_Call_CPP/include/Solution.h"

namespace org {
namespace solution {

bool Solution::canIWin(int maxChoosableInteger, int desiredTotal) const {
    if (desiredTotal <= 0) {
        return true;
    }
    if (maxChoosableInteger * (maxChoosableInteger + 1) / 2 < desiredTotal) {
        return false;
    }

    std::unordered_map<int, int> dp;

    std::function<bool(int, int)> can_win = [&](int status, int rest) {
        if (rest <= 0) {
            return false;
        }
        if (dp.count(status)) {
            return dp[status] == 1;
        }
        for (int i = 1; i <= maxChoosableInteger; i++) {
            if ((status & (1 << i)) == 0 && !can_win(status | (1 << i), rest - i)) {
                dp[status] = 1;
                return true;
            }
        }
        dp[status] = -1;
        return false;
    };

    return can_win(0, desiredTotal);
}

// 实现 new_solution 函数
std::unique_ptr<Solution> new_solution() {
    return std::make_unique<Solution>();
}

}
}