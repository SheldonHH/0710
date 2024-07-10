use std::process::Command;

fn main() {
    let mut build = cxx_build::bridge("src/lib.rs");
    
    build
        .file("src/Memory.cc")
        .flag_if_supported("-fsanitize=address")
        .flag_if_supported("-fno-omit-frame-pointer");

    build.compile("cxxbridge-memory");

    // Link the AddressSanitizer runtime
    println!("cargo:rustc-link-lib=asan");

    // Ensure we rerun the build script if these files change
    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/Memory.cc");
    println!("cargo:rerun-if-changed=include/Memory.h");
}
