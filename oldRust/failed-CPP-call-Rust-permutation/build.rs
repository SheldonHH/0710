fn main() {
    cxx_build::bridge("src/lib.rs")
        .file("src/main.cpp")
        .flag_if_supported("-std=c++14")
        .compile("success-cpp-call-rust-permutation");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/main.cpp");
    println!("cargo:rerun-if-changed=include/rust_functions.h");
}
