use std::env;

#[cxx::bridge(namespace = "org::solution")]
mod ffi {
    unsafe extern "C++" {
        include!("Permutation_Rust_Call_CPP/include/Solution.h");

        type Solution;

        fn new_solution() -> UniquePtr<Solution>;
        fn permute(self: &Solution, nums_str: &str) -> String;
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

    // 获取输入的逗号分隔字符串
    let nums_str = &args[1];

    // 调用 C++ 的 permute 方法
    let result_str = solution.permute(nums_str);

    // 解析输出并打印
    for perm in result_str.split(';') {
        println!("{}", perm);
    }
}
