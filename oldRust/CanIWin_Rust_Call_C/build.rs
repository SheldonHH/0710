use std::env;

fn main() {
    let target = env::var("CARGO_CFG_TARGET_ARCH").unwrap_or_default();
    if target == "bpf" {
        // 设置 BPF 工具链路径
        let sdk_path = env::var("BPF_SDK_PATH").unwrap_or_else(|_| {
            format!("{}/.local/share/solana/install/active_release/bin/sdk/bpf", env::var("HOME").unwrap())
        });
        println!("cargo:rustc-env=BPF_SDK_PATH={}", sdk_path);
        println!("cargo:rustc-cfg=target_os=\"solana\"");
    } else {
        // 非 BPF 目标时，正常使用 cxx_build 编译 C++ 代码
        cxx_build::bridge("src/main.rs")
            .file("src/Solution.cc")
            .flag_if_supported("-std=c++14")
            .compile("cxxbridge-solution");
    }

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/Solution.cc");
    println!("cargo:rerun-if-changed=include/Solution.h");
}
