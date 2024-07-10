#include <iostream>
#include <cstring>

int globalVar = 42; // 数据段（已初始化）

class Base {
public:
    virtual void func1() { std::cout << "Base::func1" << std::endl; }
    virtual void func2() { std::cout << "Base::func2" << std::endl; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void func1() override { std::cout << "Derived::func1" << std::endl; }
    void func2() override { std::cout << "Derived::func2" << std::endl; }
};

void printMemoryInfo(const char* name, const void* address, const char* description) {
    std::cout << name << " at address: " << address << std::endl;
    std::cout << description << std::endl << std::endl;
}

int main() {
    static int staticVar = 100; // 数据段（已初始化）
    const char* str = "Hello, World!"; // 常量区
    int localVar = 10; // 栈区

    int* heapVar = new int[10]; // 堆区
    std::memset(heapVar, 0, 10 * sizeof(int));

    std::cout << "Global Var: " << globalVar << std::endl;
    std::cout << "Static Var: " << staticVar << std::endl;
    std::cout << "Local Var: " << localVar << std::endl;
    std::cout << "Heap Var[0]: " << heapVar[0] << std::endl;
    std::cout << "String: " << str << std::endl << std::endl;

    // 创建对象并打印虚函数表指针地址
    Derived d;
    Base* basePtr = &d;

    std::cout << "Base pointer points to Derived object, calling virtual functions:" << std::endl;
    basePtr->func1();
    basePtr->func2();

    // 打印各区域的地址和描述
    printMemoryInfo("Code Segment (main function)", (void*)main, 
        "1. 代码段（Code Segment）\n英文名称: Code Segment\n功能: 存储程序的可执行代码。这个区域通常是只读的，以防止程序意外修改其指令。\n示例: 所有函数体内的代码都存储在这个区域。");

    printMemoryInfo("Initialized Data Segment (globalVar)", &globalVar, 
        "2. 数据段（Data Segment）\n英文名称: Data Segment\n功能: 存储全局变量和静态变量，包括已初始化和未初始化的部分。\n已初始化数据段（Initialized Data Segment）: 存储已初始化的全局变量和静态变量。\n示例: int globalVar = 42; 存储在数据段。");

    printMemoryInfo("Initialized Data Segment (staticVar)", &staticVar, 
        "2. 数据段（Data Segment）\n英文名称: Data Segment\n功能: 存储全局变量和静态变量，包括已初始化和未初始化的部分。\n已初始化数据段（Initialized Data Segment）: 存储已初始化的全局变量和静态变量。\n示例: static int staticVar = 100; 存储在数据段。");

    printMemoryInfo("Read-Only Data Segment (str)", str, 
        "5. 常量区（Read-Only Data Segment）\n英文名称: Read-Only Data Segment\n功能: 存储常量值（如字符串常量）。这个区域通常是只读的，以防止程序修改这些常量。\n示例: 字符串常量 \"Hello, World!\" 存储在常量区。");

    printMemoryInfo("Stack (localVar)", &localVar, 
        "3. 栈区（Stack）\n英文名称: Stack\n功能: 存储局部变量和函数调用的上下文（如函数参数、返回地址和局部变量）。栈的内存是由编译器自动分配和释放的。\n特点: 栈内存通常是高速的，但空间有限。栈的内存分配是按照后进先出（LIFO）的顺序进行的。\n示例: 函数内部定义的变量 int localVar; 存储在栈区。");

    printMemoryInfo("Heap (heapVar)", heapVar, 
        "4. 堆区（Heap）\n英文名称: Heap\n功能: 用于动态分配内存。由程序员使用 new 和 delete（或 malloc 和 free）进行手动分配和释放。\n特点: 堆内存通常比栈内存更大，但分配和释放的速度较慢，需要程序员手动管理，以防止内存泄漏。\n示例: 动态分配的内存 int* ptr = new int[10]; 存储在堆区。");

    // 打印虚函数表指针地址
    void** vptr = *(void***)&d;
    printMemoryInfo("Vtable pointer (vptr) of Derived object", vptr, 
        "虚函数表指针（vptr）存储在对象内存布局的起始位置。每个包含虚函数的对象都有一个虚函数指针，指向该类的虚函数表。");

    // 打印虚函数表地址和内容
    printMemoryInfo("Vtable of Derived class", vptr, 
        "虚函数表（vtable）存储在全局段。它是一个包含虚函数地址的全局指针数组。");
    std::cout << "Vtable contents:" << std::endl;
    for (int i = 0; vptr[i] != nullptr; ++i) {
        std::cout << "Function " << i << " at address: " << vptr[i] << std::endl;
    }

    delete[] heapVar;

    return 0;
}
