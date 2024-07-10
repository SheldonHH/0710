use std::{thread, time::Duration};

#[cxx::bridge]
mod ffi {
    extern "C++" {
        include!("cpp_memory_relation/include/Memory.h");

        type Memory;
        fn new_memory() -> Box<Memory>;
        fn allocate_memory(&self, size: usize);
        fn deallocate_memory(&self);
        fn get_ptr(&self) -> *const u8;
    }
}

pub struct Memory {
    ptr: Option<Box<[u8]>>,
    deallocate_thread: Option<std::thread::JoinHandle<()>>,
}

impl Memory {
    pub fn allocate_memory(&mut self, size: usize) {
        self.ptr = Some(vec![0u8; size].into_boxed_slice());
        let ptr_clone = self.ptr.clone();
        self.deallocate_thread = Some(std::thread::spawn(move || {
            std::thread::sleep(Duration::from_secs(1));
            drop(ptr_clone);
        }));
    }

    pub fn deallocate_memory(&mut self) {
        self.ptr = None;
    }

    pub fn get_ptr(&self) -> *const u8 {
        match &self.ptr {
            Some(ptr) => ptr.as_ptr(),
            None => std::ptr::null(),
        }
    }
}

#[no_mangle]
pub extern "C" fn new_memory() -> Box<ffi::Memory> {
    Box::new(Memory {
        ptr: None,
        deallocate_thread: None,
    })
}

#[no_mangle]
pub extern "C" fn allocate_memory(memory: &mut ffi::Memory, size: usize) {
    memory.allocate_memory(size);
}

#[no_mangle]
pub extern "C" fn deallocate_memory(memory: &mut ffi::Memory) {
    memory.deallocate_memory();
}

#[no_mangle]
pub extern "C" fn get_ptr(memory: &ffi::Memory) -> *const u8 {
    memory.get_ptr()
}
