
- 作用：静态成员变量属于类本身，而不是类的任何对象实例。所有对象实例共享

```cpp
#include <iostream>  
  
class MyClass {  
public:  
    static int count; // 静态成员变量  
  
    MyClass() {  
        count++;  
    }  
};  
  
int MyClass::count = 0; // 静态成员变量的初始化在类外完成  
  
int main() {  
    MyClass obj1, obj2;  
    std::cout << "Number of MyClass objects: " << MyClass::count << std::endl; // 输出：2  
    return 0;  
}
```

## 4. static member function
```cpp
#include <iostream>  
  
class MyClass {  
public:  
    static int count;  
    static void incrementCount() {  
        count++;  
    }  
};  
  
int MyClass::count = 0;  
  
int main() {  
    MyClass::incrementCount();  
    MyClass::incrementCount();  
    std::cout << "Count: " << MyClass::count << std::endl; // 输出：2  
    return 0;  
}
```