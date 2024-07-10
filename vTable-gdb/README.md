```bash

g++ -g -o employee_vtable employee_vtable.cpp
gdb ./employee_vtable



```


# Check Memory Layout of Object in GDB
```bash 
print &m
# 查看 Manager 对象的 vtable：
# 
print *reinterpret_cast<void**>(0x7fffffffdea8)

# &m 返回对象 m 的地址。
# *(void**) &m 将对象地址 0x7fffffffdea8 转换为 void**，并解引用这个指针，从而获取 vtable 指针。
print *(void**) &m

```

