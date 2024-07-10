use std::{thread, time::Duration};
use std::env;
use std::convert::TryInto;

#[cxx::bridge(namespace = "org::solution")]
mod ffi {
    unsafe extern "C++" {
        include!("Permutation_Rust_Call_CPP/include/Solution.h");

        type Solution;

        fn new_solution() -> UniquePtr<Solution>;
        fn permute(self: &Solution, nums: &Vec<i32>) -> Vec<Vec<i32>>;
    }
}


fn main() {
    let solution = ffi::new_solution();

    // 获取命令行参数
    let args: Vec<String> = env::args().collect();

    // 确保传递了正确数量的参数
    if args.len() != 2 {
        eprintln!("Usage: {} <comma_separated_numbers>", args[0]);
        std::process::exit(1);
    }

    // 解析输入参数
    let nums: Vec<i32> = args[1].split(',')
                                .map(|s| s.parse().expect("Invalid number"))
                                .collect();

    // 调用 C++ 的 permute 方法
    let result = solution.permute(&nums);

    // 打印结果
    for permutation in result {
        for num in permutation {
            print!("{} ", num);
        }
        println!();
    }
}
