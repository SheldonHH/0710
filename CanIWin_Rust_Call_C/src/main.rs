use std::{thread, time::Duration};


#[cxx::bridge(namespace = "org::solution")]
mod ffi {
    unsafe extern "C++" {
        include!("CanIWin_Rust_Call_CPP/include/Solution.h");

        type Solution;

        fn new_solution() -> UniquePtr<Solution>;
        fn canIWin(self: &Solution, max_choosable_integer: i32, desired_total: i32) -> bool;
    }
}

use std::env;
use std::convert::TryInto;
fn main() {
    let solution = ffi::new_solution();
    // 获取命令行参数
    let args: Vec<String> = env::args().collect();

    // 确保传递了正确数量的参数
    if args.len() != 3 {
        eprintln!("Usage: {} <max_choosable_integer> <desired_total>", args[0]);
        std::process::exit(1);
    }

    // 解析命令行参数
    let max_choosable_integer: u32 = args[1].parse().expect("Invalid number for max_choosable_integer");
    let desired_total: u32 = args[2].parse().expect("Invalid number for desired_total");
    // 打印传入的参数
    println!("Max choosable integer: {}", max_choosable_integer);
    println!("Desired total: {}", desired_total);

    let result = solution.canIWin(max_choosable_integer.try_into().unwrap(), desired_total.try_into().unwrap());
    println!("Can I win? {}", result);
}