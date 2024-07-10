fn main() {
    cxx_build::bridge("src/main.rs")
        .file("src/Memory.cc")
        .flag_if_supported("-std=c++14")
        .include("include")
        .compile("cxxbridge-memory");

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/Memory.cc");
    println!("cargo:rerun-if-changed=include/Memory.h");
}
