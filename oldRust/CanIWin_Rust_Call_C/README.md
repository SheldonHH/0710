
https://leetcode.com/problems/can-i-win/solutions/5408637/can-win/



具体题目可以概述为：两个玩家交替选择从1到maxChoosableInteger中的一个整数，已经选择过的数不能再选择，目标是第一个达到或超过desiredTotal的玩家获胜。我们需要判断先手玩家能否保证胜利。


why not Rust
```rust []
use std::collections::HashMap;
impl Solution {
    pub fn can_i_win(max_choosable_integer: i32, desired_total: i32) -> bool {
        if desired_total <= 0 {
            return true;
        }
        if max_choosable_integer * (max_choosable_integer + 1) / 2 < desired_total {
            return false;
        }

        let mut dp: HashMap<i32, bool> = HashMap::new();

        fn can_win(status: i32, rest: i32, dp: &mut HashMap<i32, bool>, max_choosable_integer: i32) -> bool {
            if rest <= 0 {
                return false;
            }
            if let Some(&result) = dp.get(&status) {
                return result;
            }
            for i in 1..=max_choosable_integer {
                if (status & (1 << i)) == 0 && !can_win(status | (1 << i), rest - i, dp, max_choosable_integer) {
                    dp.insert(status, true);
                    return true;
                }
            }
            dp.insert(status, false);
            return false;
        }

        can_win(0, desired_total, &mut dp, max_choosable_integer)
    }
}
```
# CanIWin_Rust_Call_CPP
