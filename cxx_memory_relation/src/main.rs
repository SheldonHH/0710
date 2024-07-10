use std::{thread, time::Duration};


#[cxx::bridge(namespace = "org::memory")]
mod ffi {
    unsafe extern "C++" {
        include!("cxx_memory_relation/include/Memory.h");
        
        type Memory;

        fn new_memory() -> UniquePtr<Memory>;

        fn allocate_memory(&self, size: usize);
        unsafe fn deallocate_memory(&self);
        fn get_ptr(&self) -> *const u8;
    }
}



fn main() {
    let size = 32;

    unsafe {
        let mem = ffi::new_memory();

        println!("========================   C++   ========================");
        println!("- Pointer before memory allocation: {:?}", mem.get_ptr());
        mem.allocate_memory(size);
        let ptr = mem.get_ptr();
        println!("- Allocated memory at: {:?}, value is {:?}", ptr,std::ptr::read(ptr));

        // Wait for 2 seconds for the memory at pointer to be deallocated on the C++ side
        println!("Deallocating memory");
        thread::sleep(Duration::from_secs(2));
        println!("- Checking value ({:?}) of pointer: {:?}", std::ptr::read(ptr),ptr);
        println!("=========================================================");

        println!("\n========================   Rust   ========================");
        let ptr = ptr as *mut u32;
        println!("- Data at pointer ({:?}): {:?}", ptr, std::ptr::read(ptr));

        let n = 12;
        println!("Writing '{:?}' to memory", n);
        std::ptr::write(ptr, n);
        println!("- Checking value at pointer ({:?}): {:?}", ptr, std::ptr::read(ptr));
        println!("=========================================================");
    }
}
