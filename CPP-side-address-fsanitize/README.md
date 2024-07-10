在构建项目之前，设置必要的环境变量：

```bash
export RUSTFLAGS="-Z sanitizer=address"
export ASAN_OPTIONS=detect_stack_use_after_return=1
export ASAN_SYMBOLIZER_PATH=$(which llvm-symbolizer)
export LD_PRELOAD=$(rustc --print target-libdir)/libclang_rt.asan-x86_64.so

# 清理旧的构建
cargo clean

# 构建项目
cargo build


```


# 成功发现CXX 中可以用C++端的AddressSanitanizer
```bash

  =================================================================
  ==88534==ERROR: LeakSanitizer: detected memory leaks

  Direct leak of 1952 byte(s) in 61 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x2b53000187bdbbd8  (<unknown module>)
      #3 0xc176000187bda4ec  (<unknown module>)
      #4 0x1e14800187c3e288  (<unknown module>)
      #5 0xa622800187c342f4  (<unknown module>)
      #6 0xd868000187c3acac  (<unknown module>)
      #7 0x6c40800187c6310c  (<unknown module>)
      #8 0x386c800187bda25c  (<unknown module>)
      #9 0x737800187dfbc8c  (<unknown module>)
      #10 0x5a21800187e0b568  (<unknown module>)
      #11 0x420500019483e66c  (<unknown module>)
      #12 0xe969000187c49a20  (<unknown module>)
      #13 0x506800187c8f324  (<unknown module>)
      #14 0xf94c000187c82664  (<unknown module>)
      #15 0x573c000187c292f8  (<unknown module>)
      #16 0x7175000187c8169c  (<unknown module>)
      #17 0x2478800187c8ee38  (<unknown module>)
      #18 0xd519000187c45b34  (<unknown module>)
      #19 0x4610800187c4f924  (<unknown module>)
      #20 0x1d72000187c6b35c  (<unknown module>)
      #21 0xb575800187c2df9c  (<unknown module>)
      #22 0x7467000187c2ced8  (<unknown module>)
      #23 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 1952 byte(s) in 61 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x507b000187bdd3ac  (<unknown module>)
      #3 0x2b53000187bdbbd8  (<unknown module>)
      #4 0xc176000187bda4ec  (<unknown module>)
      #5 0x1e14800187c3e288  (<unknown module>)
      #6 0xa622800187c342f4  (<unknown module>)
      #7 0xd868000187c3acac  (<unknown module>)
      #8 0x6c40800187c6310c  (<unknown module>)
      #9 0x386c800187bda25c  (<unknown module>)
      #10 0x737800187dfbc8c  (<unknown module>)
      #11 0x5a21800187e0b568  (<unknown module>)
      #12 0x420500019483e66c  (<unknown module>)
      #13 0xe969000187c49a20  (<unknown module>)
      #14 0x506800187c8f324  (<unknown module>)
      #15 0xf94c000187c82664  (<unknown module>)
      #16 0x573c000187c292f8  (<unknown module>)
      #17 0x7175000187c8169c  (<unknown module>)
      #18 0x2478800187c8ee38  (<unknown module>)
      #19 0xd519000187c45b34  (<unknown module>)
      #20 0x4610800187c4f924  (<unknown module>)
      #21 0x1d72000187c6b35c  (<unknown module>)
      #22 0xb575800187c2df9c  (<unknown module>)
      #23 0x7467000187c2ced8  (<unknown module>)
      #24 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 160 byte(s) in 5 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x7949000187bdd394  (<unknown module>)
      #3 0x2b53000187bdbbd8  (<unknown module>)
      #4 0xc176000187bda4ec  (<unknown module>)
      #5 0x1e14800187c3e288  (<unknown module>)
      #6 0xa622800187c342f4  (<unknown module>)
      #7 0xd868000187c3acac  (<unknown module>)
      #8 0x6c40800187c6310c  (<unknown module>)
      #9 0x386c800187bda25c  (<unknown module>)
      #10 0x737800187dfbc8c  (<unknown module>)
      #11 0x5a21800187e0b568  (<unknown module>)
      #12 0x420500019483e66c  (<unknown module>)
      #13 0xe969000187c49a20  (<unknown module>)
      #14 0x506800187c8f324  (<unknown module>)
      #15 0xf94c000187c82664  (<unknown module>)
      #16 0x573c000187c292f8  (<unknown module>)
      #17 0x7175000187c8169c  (<unknown module>)
      #18 0x2478800187c8ee38  (<unknown module>)
      #19 0xd519000187c45b34  (<unknown module>)
      #20 0x4610800187c4f924  (<unknown module>)
      #21 0x1d72000187c6b35c  (<unknown module>)
      #22 0xb575800187c2df9c  (<unknown module>)
      #23 0x7467000187c2ced8  (<unknown module>)
      #24 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 160 byte(s) in 5 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x156000187bdd3ac  (<unknown module>)
      #3 0x7949000187bdd394  (<unknown module>)
      #4 0x2b53000187bdbbd8  (<unknown module>)
      #5 0xc176000187bda4ec  (<unknown module>)
      #6 0x1e14800187c3e288  (<unknown module>)
      #7 0xa622800187c342f4  (<unknown module>)
      #8 0xd868000187c3acac  (<unknown module>)
      #9 0x6c40800187c6310c  (<unknown module>)
      #10 0x386c800187bda25c  (<unknown module>)
      #11 0x737800187dfbc8c  (<unknown module>)
      #12 0x5a21800187e0b568  (<unknown module>)
      #13 0x420500019483e66c  (<unknown module>)
      #14 0xe969000187c49a20  (<unknown module>)
      #15 0x506800187c8f324  (<unknown module>)
      #16 0xf94c000187c82664  (<unknown module>)
      #17 0x573c000187c292f8  (<unknown module>)
      #18 0x7175000187c8169c  (<unknown module>)
      #19 0x2478800187c8ee38  (<unknown module>)
      #20 0xd519000187c45b34  (<unknown module>)
      #21 0x4610800187c4f924  (<unknown module>)
      #22 0x1d72000187c6b35c  (<unknown module>)
      #23 0xb575800187c2df9c  (<unknown module>)
      #24 0x7467000187c2ced8  (<unknown module>)
      #25 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 104 byte(s) in 1 object(s) allocated from:
      #0 0x102b9abb4 in wrap_malloc+0x88 (librustc-nightly_rt.asan.dylib:arm64+0x3ebb4) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187c3b788  (<unknown module>)
      #2 0x587d800187fb6688  (<unknown module>)
      #3 0x2e1a80010239538c  (<unknown module>)
      #4 0x1023908f4 in std::rt::lang_start_internal::h9b5609abf7bda146+0x278 (build-script-build:arm64+0x1000208f4) (BuildId: 92987493b62736ca91af73ce6345c37232000000200000000100000000000e00)
      #5 0x10237d288 in std::rt::lang_start::h2f33e7e7a728506b rt.rs:165
      #6 0x102377e54 in main+0x20 (build-script-build:arm64+0x100007e54) (BuildId: 92987493b62736ca91af73ce6345c37232000000200000000100000000000e00)
      #7 0x187c2d0dc  (<unknown module>)
      #8 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 72 byte(s) in 1 object(s) allocated from:
      #0 0x102b9abb4 in wrap_malloc+0x88 (librustc-nightly_rt.asan.dylib:arm64+0x3ebb4) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187be265c in _fetchInitializingClassList(bool)+0x2c (libobjc.A.dylib:arm64+0xa65c) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x6170800187be2530  (<unknown module>)
      #3 0xe12800187be2374  (<unknown module>)
      #4 0x4b53000187be21ec  (<unknown module>)
      #5 0x437800187be21ec  (<unknown module>)
      #6 0x6212800187be21ec  (<unknown module>)
      #7 0x2620000187bff300  (<unknown module>)
      #8 0x7e5b800187be1dc0  (<unknown module>)
      #9 0x8538800187be1760  (<unknown module>)
      #10 0x9f52800187cc3444  (<unknown module>)
      #11 0xcb37800187cc2800  (<unknown module>)
      #12 0x9c1400019483e678  (<unknown module>)
      #13 0xe969000187c49a20  (<unknown module>)
      #14 0x506800187c8f324  (<unknown module>)
      #15 0xf94c000187c82664  (<unknown module>)
      #16 0x573c000187c292f8  (<unknown module>)
      #17 0x7175000187c8169c  (<unknown module>)
      #18 0x2478800187c8ee38  (<unknown module>)
      #19 0xd519000187c45b34  (<unknown module>)
      #20 0x4610800187c4f924  (<unknown module>)
      #21 0x1d72000187c6b35c  (<unknown module>)
      #22 0xb575800187c2df9c  (<unknown module>)
      #23 0x7467000187c2ced8  (<unknown module>)
      #24 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 64 byte(s) in 2 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xc67e000187bdd3ac  (<unknown module>)
      #3 0x970a000187bdd394  (<unknown module>)
      #4 0x7949000187bdd394  (<unknown module>)
      #5 0x2b53000187bdbbd8  (<unknown module>)
      #6 0xc176000187bda4ec  (<unknown module>)
      #7 0x1e14800187c3e288  (<unknown module>)
      #8 0xa622800187c342f4  (<unknown module>)
      #9 0xd868000187c3acac  (<unknown module>)
      #10 0x6c40800187c6310c  (<unknown module>)
      #11 0x386c800187bda25c  (<unknown module>)
      #12 0x737800187dfbc8c  (<unknown module>)
      #13 0x5a21800187e0b568  (<unknown module>)
      #14 0x420500019483e66c  (<unknown module>)
      #15 0xe969000187c49a20  (<unknown module>)
      #16 0x506800187c8f324  (<unknown module>)
      #17 0xf94c000187c82664  (<unknown module>)
      #18 0x573c000187c292f8  (<unknown module>)
      #19 0x7175000187c8169c  (<unknown module>)
      #20 0x2478800187c8ee38  (<unknown module>)
      #21 0xd519000187c45b34  (<unknown module>)
      #22 0x4610800187c4f924  (<unknown module>)
      #23 0x1d72000187c6b35c  (<unknown module>)
      #24 0xb575800187c2df9c  (<unknown module>)
      #25 0x7467000187c2ced8  (<unknown module>)
      #26 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 64 byte(s) in 2 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x970a000187bdd394  (<unknown module>)
      #3 0x7949000187bdd394  (<unknown module>)
      #4 0x2b53000187bdbbd8  (<unknown module>)
      #5 0xc176000187bda4ec  (<unknown module>)
      #6 0x1e14800187c3e288  (<unknown module>)
      #7 0xa622800187c342f4  (<unknown module>)
      #8 0xd868000187c3acac  (<unknown module>)
      #9 0x6c40800187c6310c  (<unknown module>)
      #10 0x386c800187bda25c  (<unknown module>)
      #11 0x737800187dfbc8c  (<unknown module>)
      #12 0x5a21800187e0b568  (<unknown module>)
      #13 0x420500019483e66c  (<unknown module>)
      #14 0xe969000187c49a20  (<unknown module>)
      #15 0x506800187c8f324  (<unknown module>)
      #16 0xf94c000187c82664  (<unknown module>)
      #17 0x573c000187c292f8  (<unknown module>)
      #18 0x7175000187c8169c  (<unknown module>)
      #19 0x2478800187c8ee38  (<unknown module>)
      #20 0xd519000187c45b34  (<unknown module>)
      #21 0x4610800187c4f924  (<unknown module>)
      #22 0x1d72000187c6b35c  (<unknown module>)
      #23 0xb575800187c2df9c  (<unknown module>)
      #24 0x7467000187c2ced8  (<unknown module>)
      #25 0xad7e7ffffffffffc  (<unknown module>)

  Direct leak of 32 byte(s) in 1 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bfa5a0 in cache_t::insert(objc_selector*, void (*)(), objc_object*)+0x178 (libobjc.A.dylib:arm64+0x225a0) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xc460800187be1c8c  (<unknown module>)
      #3 0x24e800187be1760  (<unknown module>)
      #4 0x104f800187cc5a68  (<unknown module>)
      #5 0x9f52800187cc3444  (<unknown module>)
      #6 0xcb37800187cc2800  (<unknown module>)
      #7 0x9c1400019483e678  (<unknown module>)
      #8 0xe969000187c49a20  (<unknown module>)
      #9 0x506800187c8f324  (<unknown module>)
      #10 0xf94c000187c82664  (<unknown module>)
      #11 0x573c000187c292f8  (<unknown module>)
      #12 0x7175000187c8169c  (<unknown module>)
      #13 0x2478800187c8ee38  (<unknown module>)
      #14 0xd519000187c45b34  (<unknown module>)
      #15 0x4610800187c4f924  (<unknown module>)
      #16 0x1d72000187c6b35c  (<unknown module>)
      #17 0xb575800187c2df9c  (<unknown module>)
      #18 0x7467000187c2ced8  (<unknown module>)
      #19 0xad7e7ffffffffffc  (<unknown module>)

  Indirect leak of 48 byte(s) in 1 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bfc920 in class_rw_t::extAlloc(class_ro_t const*, bool)+0x38 (libobjc.A.dylib:arm64+0x24920) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xb97a000187bddc38  (<unknown module>)
      #3 0x7949000187bdd394  (<unknown module>)
      #4 0x2b53000187bdbbd8  (<unknown module>)
      #5 0xc176000187bda4ec  (<unknown module>)
      #6 0x1e14800187c3e288  (<unknown module>)
      #7 0xa622800187c342f4  (<unknown module>)
      #8 0xd868000187c3acac  (<unknown module>)
      #9 0x6c40800187c6310c  (<unknown module>)
      #10 0x386c800187bda25c  (<unknown module>)
      #11 0x737800187dfbc8c  (<unknown module>)
      #12 0x5a21800187e0b568  (<unknown module>)
      #13 0x420500019483e66c  (<unknown module>)
      #14 0xe969000187c49a20  (<unknown module>)
      #15 0x506800187c8f324  (<unknown module>)
      #16 0xf94c000187c82664  (<unknown module>)
      #17 0x573c000187c292f8  (<unknown module>)
      #18 0x7175000187c8169c  (<unknown module>)
      #19 0x2478800187c8ee38  (<unknown module>)
      #20 0xd519000187c45b34  (<unknown module>)
      #21 0x4610800187c4f924  (<unknown module>)
      #22 0x1d72000187c6b35c  (<unknown module>)
      #23 0xb575800187c2df9c  (<unknown module>)
      #24 0x7467000187c2ced8  (<unknown module>)
      #25 0xad7e7ffffffffffc  (<unknown module>)

  Indirect leak of 32 byte(s) in 1 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187be26b8 in _fetchInitializingClassList(bool)+0x88 (libobjc.A.dylib:arm64+0xa6b8) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x6170800187be2530  (<unknown module>)
      #3 0xe12800187be2374  (<unknown module>)
      #4 0x4b53000187be21ec  (<unknown module>)
      #5 0x437800187be21ec  (<unknown module>)
      #6 0x6212800187be21ec  (<unknown module>)
      #7 0x2620000187bff300  (<unknown module>)
      #8 0x7e5b800187be1dc0  (<unknown module>)
      #9 0x8538800187be1760  (<unknown module>)
      #10 0x9f52800187cc3444  (<unknown module>)
      #11 0xcb37800187cc2800  (<unknown module>)
      #12 0x9c1400019483e678  (<unknown module>)
      #13 0xe969000187c49a20  (<unknown module>)
      #14 0x506800187c8f324  (<unknown module>)
      #15 0xf94c000187c82664  (<unknown module>)
      #16 0x573c000187c292f8  (<unknown module>)
      #17 0x7175000187c8169c  (<unknown module>)
      #18 0x2478800187c8ee38  (<unknown module>)
      #19 0xd519000187c45b34  (<unknown module>)
      #20 0x4610800187c4f924  (<unknown module>)
      #21 0x1d72000187c6b35c  (<unknown module>)
      #22 0xb575800187c2df9c  (<unknown module>)
      #23 0x7467000187c2ced8  (<unknown module>)
      #24 0xad7e7ffffffffffc  (<unknown module>)

  Indirect leak of 16 byte(s) in 1 object(s) allocated from:
      #0 0x102b9af5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187be2694 in _fetchInitializingClassList(bool)+0x64 (libobjc.A.dylib:arm64+0xa694) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x6170800187be2530  (<unknown module>)
      #3 0xe12800187be2374  (<unknown module>)
      #4 0x4b53000187be21ec  (<unknown module>)
      #5 0x437800187be21ec  (<unknown module>)
      #6 0x6212800187be21ec  (<unknown module>)
      #7 0x2620000187bff300  (<unknown module>)
      #8 0x7e5b800187be1dc0  (<unknown module>)
      #9 0x8538800187be1760  (<unknown module>)
      #10 0x9f52800187cc3444  (<unknown module>)
      #11 0xcb37800187cc2800  (<unknown module>)
      #12 0x9c1400019483e678  (<unknown module>)
      #13 0xe969000187c49a20  (<unknown module>)
      #14 0x506800187c8f324  (<unknown module>)
      #15 0xf94c000187c82664  (<unknown module>)
      #16 0x573c000187c292f8  (<unknown module>)
      #17 0x7175000187c8169c  (<unknown module>)
      #18 0x2478800187c8ee38  (<unknown module>)
      #19 0xd519000187c45b34  (<unknown module>)
      #20 0x4610800187c4f924  (<unknown module>)
      #21 0x1d72000187c6b35c  (<unknown module>)
      #22 0xb575800187c2df9c  (<unknown module>)
      #23 0x7467000187c2ced8  (<unknown module>)
      #24 0xad7e7ffffffffffc  (<unknown module>)

  SUMMARY: AddressSanitizer: 4656 byte(s) leaked in 142 allocation(s).
error: failed to run custom build command for `link-cplusplus v1.0.9`

Caused by:
  process didn't exit successfully: `/Users/mac/trading-programming/R2C/cpp_memory_relation/target/debug/build/link-cplusplus-b8bf77575cee7445/build-script-build` (exit status: 1)
  --- stdout
  cargo:rerun-if-changed=build.rs
  OUT_DIR = Some(/Users/mac/trading-programming/R2C/cpp_memory_relation/target/debug/build/link-cplusplus-912e797cd3f55334/out)
  TARGET = Some(aarch64-apple-darwin)
  OPT_LEVEL = Some(0)
  HOST = Some(aarch64-apple-darwin)
  cargo:rerun-if-env-changed=CXX_aarch64-apple-darwin
  CXX_aarch64-apple-darwin = None
  cargo:rerun-if-env-changed=CXX_aarch64_apple_darwin
  CXX_aarch64_apple_darwin = None
  cargo:rerun-if-env-changed=HOST_CXX
  HOST_CXX = None
  cargo:rerun-if-env-changed=CXX
  CXX = None
  cargo:rerun-if-env-changed=CC_ENABLE_DEBUG_OUTPUT
  RUSTC_WRAPPER = None
  cargo:rerun-if-env-changed=CRATE_CC_NO_DEFAULTS
  CRATE_CC_NO_DEFAULTS = None
  DEBUG = Some(true)
  cargo:rerun-if-env-changed=MACOSX_DEPLOYMENT_TARGET
  MACOSX_DEPLOYMENT_TARGET = None
  cargo:rerun-if-env-changed=CXXFLAGS_aarch64-apple-darwin
  CXXFLAGS_aarch64-apple-darwin = None
  cargo:rerun-if-env-changed=CXXFLAGS_aarch64_apple_darwin
  CXXFLAGS_aarch64_apple_darwin = None
  cargo:rerun-if-env-changed=HOST_CXXFLAGS
  HOST_CXXFLAGS = None
  cargo:rerun-if-env-changed=CXXFLAGS
  CXXFLAGS = None
  cargo:rerun-if-env-changed=AR_aarch64-apple-darwin
  AR_aarch64-apple-darwin = None
  cargo:rerun-if-env-changed=AR_aarch64_apple_darwin
  AR_aarch64_apple_darwin = None
  cargo:rerun-if-env-changed=HOST_AR
  HOST_AR = None
  cargo:rerun-if-env-changed=AR
  AR = None
  cargo:rerun-if-env-changed=ARFLAGS_aarch64-apple-darwin
  ARFLAGS_aarch64-apple-darwin = None
  cargo:rerun-if-env-changed=ARFLAGS_aarch64_apple_darwin
  ARFLAGS_aarch64_apple_darwin = None
  cargo:rerun-if-env-changed=HOST_ARFLAGS
  HOST_ARFLAGS = None
  cargo:rerun-if-env-changed=ARFLAGS
  ARFLAGS = None
  cargo:rustc-link-lib=static=link-cplusplus
  cargo:rustc-link-search=native=/Users/mac/trading-programming/R2C/cpp_memory_relation/target/debug/build/link-cplusplus-912e797cd3f55334/out
  cargo:rerun-if-env-changed=CXXSTDLIB_aarch64-apple-darwin
  CXXSTDLIB_aarch64-apple-darwin = None
  cargo:rerun-if-env-changed=CXXSTDLIB_aarch64_apple_darwin
  CXXSTDLIB_aarch64_apple_darwin = None
  cargo:rerun-if-env-changed=HOST_CXXSTDLIB
  HOST_CXXSTDLIB = None
  cargo:rerun-if-env-changed=CXXSTDLIB
  CXXSTDLIB = None
  cargo:rustc-link-lib=c++

  --- stderr

  =================================================================
  ==88533==ERROR: LeakSanitizer: detected memory leaks

  Direct leak of 1952 byte(s) in 61 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xbd24000187bdd3ac  (<unknown module>)
      #3 0xad26800187bdbbd8  (<unknown module>)
      #4 0xa46800187bda4ec  (<unknown module>)
      #5 0xdb5a800187c3e288  (<unknown module>)
      #6 0xd253800187c342f4  (<unknown module>)
      #7 0xe539800187c3acac  (<unknown module>)
      #8 0xe230000187c6310c  (<unknown module>)
      #9 0x1673000187bda25c  (<unknown module>)
      #10 0xb079800187dfbc8c  (<unknown module>)
      #11 0x7c03000187e0b568  (<unknown module>)
      #12 0x611680019483e66c  (<unknown module>)
      #13 0xa73d800187c49a20  (<unknown module>)
      #14 0x8938000187c8f324  (<unknown module>)
      #15 0xfa40800187c82664  (<unknown module>)
      #16 0xd266000187c292f8  (<unknown module>)
      #17 0x3349800187c8169c  (<unknown module>)
      #18 0xfa4e000187c8ee38  (<unknown module>)
      #19 0x227e000187c45b34  (<unknown module>)
      #20 0x5359800187c4f924  (<unknown module>)
      #21 0xab36800187c6b35c  (<unknown module>)
      #22 0x3f28000187c2df9c  (<unknown module>)
      #23 0x8d77000187c2ced8  (<unknown module>)
      #24 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 1952 byte(s) in 61 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xad26800187bdbbd8  (<unknown module>)
      #3 0xa46800187bda4ec  (<unknown module>)
      #4 0xdb5a800187c3e288  (<unknown module>)
      #5 0xd253800187c342f4  (<unknown module>)
      #6 0xe539800187c3acac  (<unknown module>)
      #7 0xe230000187c6310c  (<unknown module>)
      #8 0x1673000187bda25c  (<unknown module>)
      #9 0xb079800187dfbc8c  (<unknown module>)
      #10 0x7c03000187e0b568  (<unknown module>)
      #11 0x611680019483e66c  (<unknown module>)
      #12 0xa73d800187c49a20  (<unknown module>)
      #13 0x8938000187c8f324  (<unknown module>)
      #14 0xfa40800187c82664  (<unknown module>)
      #15 0xd266000187c292f8  (<unknown module>)
      #16 0x3349800187c8169c  (<unknown module>)
      #17 0xfa4e000187c8ee38  (<unknown module>)
      #18 0x227e000187c45b34  (<unknown module>)
      #19 0x5359800187c4f924  (<unknown module>)
      #20 0xab36800187c6b35c  (<unknown module>)
      #21 0x3f28000187c2df9c  (<unknown module>)
      #22 0x8d77000187c2ced8  (<unknown module>)
      #23 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 160 byte(s) in 5 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xb807000187bdd394  (<unknown module>)
      #3 0xad26800187bdbbd8  (<unknown module>)
      #4 0xa46800187bda4ec  (<unknown module>)
      #5 0xdb5a800187c3e288  (<unknown module>)
      #6 0xd253800187c342f4  (<unknown module>)
      #7 0xe539800187c3acac  (<unknown module>)
      #8 0xe230000187c6310c  (<unknown module>)
      #9 0x1673000187bda25c  (<unknown module>)
      #10 0xb079800187dfbc8c  (<unknown module>)
      #11 0x7c03000187e0b568  (<unknown module>)
      #12 0x611680019483e66c  (<unknown module>)
      #13 0xa73d800187c49a20  (<unknown module>)
      #14 0x8938000187c8f324  (<unknown module>)
      #15 0xfa40800187c82664  (<unknown module>)
      #16 0xd266000187c292f8  (<unknown module>)
      #17 0x3349800187c8169c  (<unknown module>)
      #18 0xfa4e000187c8ee38  (<unknown module>)
      #19 0x227e000187c45b34  (<unknown module>)
      #20 0x5359800187c4f924  (<unknown module>)
      #21 0xab36800187c6b35c  (<unknown module>)
      #22 0x3f28000187c2df9c  (<unknown module>)
      #23 0x8d77000187c2ced8  (<unknown module>)
      #24 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 160 byte(s) in 5 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xfd0d000187bdd3ac  (<unknown module>)
      #3 0xb807000187bdd394  (<unknown module>)
      #4 0xad26800187bdbbd8  (<unknown module>)
      #5 0xa46800187bda4ec  (<unknown module>)
      #6 0xdb5a800187c3e288  (<unknown module>)
      #7 0xd253800187c342f4  (<unknown module>)
      #8 0xe539800187c3acac  (<unknown module>)
      #9 0xe230000187c6310c  (<unknown module>)
      #10 0x1673000187bda25c  (<unknown module>)
      #11 0xb079800187dfbc8c  (<unknown module>)
      #12 0x7c03000187e0b568  (<unknown module>)
      #13 0x611680019483e66c  (<unknown module>)
      #14 0xa73d800187c49a20  (<unknown module>)
      #15 0x8938000187c8f324  (<unknown module>)
      #16 0xfa40800187c82664  (<unknown module>)
      #17 0xd266000187c292f8  (<unknown module>)
      #18 0x3349800187c8169c  (<unknown module>)
      #19 0xfa4e000187c8ee38  (<unknown module>)
      #20 0x227e000187c45b34  (<unknown module>)
      #21 0x5359800187c4f924  (<unknown module>)
      #22 0xab36800187c6b35c  (<unknown module>)
      #23 0x3f28000187c2df9c  (<unknown module>)
      #24 0x8d77000187c2ced8  (<unknown module>)
      #25 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 72 byte(s) in 1 object(s) allocated from:
      #0 0x100be2bb4 in wrap_malloc+0x88 (librustc-nightly_rt.asan.dylib:arm64+0x3ebb4) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187be265c in _fetchInitializingClassList(bool)+0x2c (libobjc.A.dylib:arm64+0xa65c) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xb314000187be2530  (<unknown module>)
      #3 0xc676000187be2374  (<unknown module>)
      #4 0xa32000187be21ec  (<unknown module>)
      #5 0x3f5e800187be21ec  (<unknown module>)
      #6 0x106e000187be21ec  (<unknown module>)
      #7 0xd709000187bff300  (<unknown module>)
      #8 0x4d4e000187be1dc0  (<unknown module>)
      #9 0xee2d800187be1760  (<unknown module>)
      #10 0x9a14000187cc3444  (<unknown module>)
      #11 0xa43a000187cc2800  (<unknown module>)
      #12 0x7a7700019483e678  (<unknown module>)
      #13 0xa73d800187c49a20  (<unknown module>)
      #14 0x8938000187c8f324  (<unknown module>)
      #15 0xfa40800187c82664  (<unknown module>)
      #16 0xd266000187c292f8  (<unknown module>)
      #17 0x3349800187c8169c  (<unknown module>)
      #18 0xfa4e000187c8ee38  (<unknown module>)
      #19 0x227e000187c45b34  (<unknown module>)
      #20 0x5359800187c4f924  (<unknown module>)
      #21 0xab36800187c6b35c  (<unknown module>)
      #22 0x3f28000187c2df9c  (<unknown module>)
      #23 0x8d77000187c2ced8  (<unknown module>)
      #24 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 64 byte(s) in 2 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xde69800187bdd3ac  (<unknown module>)
      #3 0x1d02000187bdd394  (<unknown module>)
      #4 0xb807000187bdd394  (<unknown module>)
      #5 0xad26800187bdbbd8  (<unknown module>)
      #6 0xa46800187bda4ec  (<unknown module>)
      #7 0xdb5a800187c3e288  (<unknown module>)
      #8 0xd253800187c342f4  (<unknown module>)
      #9 0xe539800187c3acac  (<unknown module>)
      #10 0xe230000187c6310c  (<unknown module>)
      #11 0x1673000187bda25c  (<unknown module>)
      #12 0xb079800187dfbc8c  (<unknown module>)
      #13 0x7c03000187e0b568  (<unknown module>)
      #14 0x611680019483e66c  (<unknown module>)
      #15 0xa73d800187c49a20  (<unknown module>)
      #16 0x8938000187c8f324  (<unknown module>)
      #17 0xfa40800187c82664  (<unknown module>)
      #18 0xd266000187c292f8  (<unknown module>)
      #19 0x3349800187c8169c  (<unknown module>)
      #20 0xfa4e000187c8ee38  (<unknown module>)
      #21 0x227e000187c45b34  (<unknown module>)
      #22 0x5359800187c4f924  (<unknown module>)
      #23 0xab36800187c6b35c  (<unknown module>)
      #24 0x3f28000187c2df9c  (<unknown module>)
      #25 0x8d77000187c2ced8  (<unknown module>)
      #26 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 64 byte(s) in 2 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bdd224 in realizeClassWithoutSwift(objc_class*, objc_class*)+0x88 (libobjc.A.dylib:arm64+0x5224) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x1d02000187bdd394  (<unknown module>)
      #3 0xb807000187bdd394  (<unknown module>)
      #4 0xad26800187bdbbd8  (<unknown module>)
      #5 0xa46800187bda4ec  (<unknown module>)
      #6 0xdb5a800187c3e288  (<unknown module>)
      #7 0xd253800187c342f4  (<unknown module>)
      #8 0xe539800187c3acac  (<unknown module>)
      #9 0xe230000187c6310c  (<unknown module>)
      #10 0x1673000187bda25c  (<unknown module>)
      #11 0xb079800187dfbc8c  (<unknown module>)
      #12 0x7c03000187e0b568  (<unknown module>)
      #13 0x611680019483e66c  (<unknown module>)
      #14 0xa73d800187c49a20  (<unknown module>)
      #15 0x8938000187c8f324  (<unknown module>)
      #16 0xfa40800187c82664  (<unknown module>)
      #17 0xd266000187c292f8  (<unknown module>)
      #18 0x3349800187c8169c  (<unknown module>)
      #19 0xfa4e000187c8ee38  (<unknown module>)
      #20 0x227e000187c45b34  (<unknown module>)
      #21 0x5359800187c4f924  (<unknown module>)
      #22 0xab36800187c6b35c  (<unknown module>)
      #23 0x3f28000187c2df9c  (<unknown module>)
      #24 0x8d77000187c2ced8  (<unknown module>)
      #25 0x3850fffffffffffc  (<unknown module>)

  Direct leak of 32 byte(s) in 1 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bfa5a0 in cache_t::insert(objc_selector*, void (*)(), objc_object*)+0x178 (libobjc.A.dylib:arm64+0x225a0) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0x9f46000187be1c8c  (<unknown module>)
      #3 0xaa09800187be1760  (<unknown module>)
      #4 0x535b000187cc5a68  (<unknown module>)
      #5 0x9a14000187cc3444  (<unknown module>)
      #6 0xa43a000187cc2800  (<unknown module>)
      #7 0x7a7700019483e678  (<unknown module>)
      #8 0xa73d800187c49a20  (<unknown module>)
      #9 0x8938000187c8f324  (<unknown module>)
      #10 0xfa40800187c82664  (<unknown module>)
      #11 0xd266000187c292f8  (<unknown module>)
      #12 0x3349800187c8169c  (<unknown module>)
      #13 0xfa4e000187c8ee38  (<unknown module>)
      #14 0x227e000187c45b34  (<unknown module>)
      #15 0x5359800187c4f924  (<unknown module>)
      #16 0xab36800187c6b35c  (<unknown module>)
      #17 0x3f28000187c2df9c  (<unknown module>)
      #18 0x8d77000187c2ced8  (<unknown module>)
      #19 0x3850fffffffffffc  (<unknown module>)

  Indirect leak of 48 byte(s) in 1 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187bfc920 in class_rw_t::extAlloc(class_ro_t const*, bool)+0x38 (libobjc.A.dylib:arm64+0x24920) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xb805800187bddc38  (<unknown module>)
      #3 0xb807000187bdd394  (<unknown module>)
      #4 0xad26800187bdbbd8  (<unknown module>)
      #5 0xa46800187bda4ec  (<unknown module>)
      #6 0xdb5a800187c3e288  (<unknown module>)
      #7 0xd253800187c342f4  (<unknown module>)
      #8 0xe539800187c3acac  (<unknown module>)
      #9 0xe230000187c6310c  (<unknown module>)
      #10 0x1673000187bda25c  (<unknown module>)
      #11 0xb079800187dfbc8c  (<unknown module>)
      #12 0x7c03000187e0b568  (<unknown module>)
      #13 0x611680019483e66c  (<unknown module>)
      #14 0xa73d800187c49a20  (<unknown module>)
      #15 0x8938000187c8f324  (<unknown module>)
      #16 0xfa40800187c82664  (<unknown module>)
      #17 0xd266000187c292f8  (<unknown module>)
      #18 0x3349800187c8169c  (<unknown module>)
      #19 0xfa4e000187c8ee38  (<unknown module>)
      #20 0x227e000187c45b34  (<unknown module>)
      #21 0x5359800187c4f924  (<unknown module>)
      #22 0xab36800187c6b35c  (<unknown module>)
      #23 0x3f28000187c2df9c  (<unknown module>)
      #24 0x8d77000187c2ced8  (<unknown module>)
      #25 0x3850fffffffffffc  (<unknown module>)

  Indirect leak of 32 byte(s) in 1 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187be26b8 in _fetchInitializingClassList(bool)+0x88 (libobjc.A.dylib:arm64+0xa6b8) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xb314000187be2530  (<unknown module>)
      #3 0xc676000187be2374  (<unknown module>)
      #4 0xa32000187be21ec  (<unknown module>)
      #5 0x3f5e800187be21ec  (<unknown module>)
      #6 0x106e000187be21ec  (<unknown module>)
      #7 0xd709000187bff300  (<unknown module>)
      #8 0x4d4e000187be1dc0  (<unknown module>)
      #9 0xee2d800187be1760  (<unknown module>)
      #10 0x9a14000187cc3444  (<unknown module>)
      #11 0xa43a000187cc2800  (<unknown module>)
      #12 0x7a7700019483e678  (<unknown module>)
      #13 0xa73d800187c49a20  (<unknown module>)
      #14 0x8938000187c8f324  (<unknown module>)
      #15 0xfa40800187c82664  (<unknown module>)
      #16 0xd266000187c292f8  (<unknown module>)
      #17 0x3349800187c8169c  (<unknown module>)
      #18 0xfa4e000187c8ee38  (<unknown module>)
      #19 0x227e000187c45b34  (<unknown module>)
      #20 0x5359800187c4f924  (<unknown module>)
      #21 0xab36800187c6b35c  (<unknown module>)
      #22 0x3f28000187c2df9c  (<unknown module>)
      #23 0x8d77000187c2ced8  (<unknown module>)
      #24 0x3850fffffffffffc  (<unknown module>)

  Indirect leak of 16 byte(s) in 1 object(s) allocated from:
      #0 0x100be2f5c in wrap_calloc+0x90 (librustc-nightly_rt.asan.dylib:arm64+0x3ef5c) (BuildId: 31bdcb1179dd3203aa5361ad5e96177032000000200000000100000000000b00)
      #1 0x187be2694 in _fetchInitializingClassList(bool)+0x64 (libobjc.A.dylib:arm64+0xa694) (BuildId: 9bab95567a2a30a8acde010ba8e2367d32000000200000000100000000020e00)
      #2 0xb314000187be2530  (<unknown module>)
      #3 0xc676000187be2374  (<unknown module>)
      #4 0xa32000187be21ec  (<unknown module>)
      #5 0x3f5e800187be21ec  (<unknown module>)
      #6 0x106e000187be21ec  (<unknown module>)
      #7 0xd709000187bff300  (<unknown module>)
      #8 0x4d4e000187be1dc0  (<unknown module>)
      #9 0xee2d800187be1760  (<unknown module>)
      #10 0x9a14000187cc3444  (<unknown module>)
      #11 0xa43a000187cc2800  (<unknown module>)
      #12 0x7a7700019483e678  (<unknown module>)
      #13 0xa73d800187c49a20  (<unknown module>)
      #14 0x8938000187c8f324  (<unknown module>)
      #15 0xfa40800187c82664  (<unknown module>)
      #16 0xd266000187c292f8  (<unknown module>)
      #17 0x3349800187c8169c  (<unknown module>)
      #18 0xfa4e000187c8ee38  (<unknown module>)
      #19 0x227e000187c45b34  (<unknown module>)
      #20 0x5359800187c4f924  (<unknown module>)
      #21 0xab36800187c6b35c  (<unknown module>)
      #22 0x3f28000187c2df9c  (<unknown module>)
      #23 0x8d77000187c2ced8  (<unknown module>)
      #24 0x3850fffffffffffc  (<unknown module>)

  SUMMARY: AddressSanitizer: 4552 byte(s) leaked in 141 allocation(s).
  ```



```bash
root@e500e0d544ab:~/trading-programming/cpp_memory_relation# find $(rustc --print sysroot) -name '*clang_rt.asan*.so'
ERROR: ld.so: object '/root/.rustup/toolchains/nightly-2023-02-05-aarch64-unknown-linux-gnu/lib/rustlib/aarch64-unknown-linux-gnu/lib/libclang_rt.asan-x86_64.so' from LD_PRELOAD cannot be preloaded (cannot open shared object file): ignored.
ERROR: ld.so: object '/root/.rustup/toolchains/nightly-2023-02-05-aarch64-unknown-linux-gnu/lib/rustlib/aarch64-unknown-linux-gnu/lib/libclang_rt.asan-x86_64.so' from LD_PRELOAD cannot be preloaded (cannot open shared object file): ignored.
ERROR: ld.so: object '/root/.rustup/toolchains/nightly-2023-02-05-aarch64-unknown-linux-gnu/lib/rustlib/aarch64-unknown-linux-gnu/lib/libclang_rt.asan-x86_64.so' from LD_PRELOAD cannot be preloaded (cannot open shared object file): ignored.
```


```bash
rustup component add rust-src
rustup component add llvm-tools-preview

```