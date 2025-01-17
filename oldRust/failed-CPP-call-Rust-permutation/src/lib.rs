use std::collections::HashSet;
use std::sync::{Mutex, MutexGuard};

lazy_static::lazy_static! {
    static ref ANS: Mutex<Vec<Vec<i32>>> = Mutex::new(Vec::new());
    static ref PATH: Mutex<Vec<i32>> = Mutex::new(Vec::new());
    static ref N: Mutex<usize> = Mutex::new(0);
}

pub struct Solution;

impl Solution {
    pub fn permute(nums: Vec<i32>) -> Vec<String> {
        Solution::set_n(nums.len());
        Solution::resize_path(*Solution::get_n());

        fn dfs(i: usize, s: HashSet<i32>) {
            if i == *Solution::get_n() {
                Solution::add_to_ans(Solution::get_path().clone());
                return;
            }
            for &x in &s {
                Solution::set_path(i, x);
                let mut new_s = s.clone();
                new_s.remove(&x);
                dfs(i + 1, new_s);
            }
        }

        let s: HashSet<i32> = nums.into_iter().collect();
        dfs(0, s);

        Solution::get_ans()
            .iter()
            .map(|v| v.iter().map(|i| i.to_string()).collect::<Vec<_>>().join(","))
            .collect()
    }

    fn set_n(val: usize) {
        let mut n = N.lock().unwrap();
        *n = val;
    }

    fn get_n() -> MutexGuard<'static, usize> {
        N.lock().unwrap()
    }

    fn resize_path(size: usize) {
        let mut path = PATH.lock().unwrap();
        path.resize(size, 0);
    }

    fn set_path(index: usize, val: i32) {
        let mut path = PATH.lock().unwrap();
        path[index] = val;
    }

    fn get_path() -> MutexGuard<'static, Vec<i32>> {
        PATH.lock().unwrap()
    }

    fn add_to_ans(val: Vec<i32>) {
        let mut ans = ANS.lock().unwrap();
        ans.push(val);
    }

    fn get_ans() -> MutexGuard<'static, Vec<Vec<i32>>> {
        ANS.lock().unwrap()
    }
}

#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn permute(nums: Vec<i32>) -> Vec<String>;
    }
}

fn permute(nums: Vec<i32>) -> Vec<String> {
    Solution::permute(nums)
}
