#include <iostream>
#include <memory>
#include <locale>

// 资源类，模拟需要管理的资源
class Resource {
public:
    Resource() {
        std::cout << "Resource acquired\n";  // 构造函数，资源获取时打印信息
    }

    ~Resource() {
        std::cout << "Resource released\n";  // 析构函数，资源释放时打印信息
    }

    void say_hello() const {
        std::cout << "Resource says: Hello\n";  // 模拟资源的方法
    }
};

// 演示std::unique_ptr的使用
void uniquePtrExample() {
    // std::unique_ptr是独占所有权的智能指针，不允许多个指针指向同一资源
    std::unique_ptr<Resource> uniqueRes = std::make_unique<Resource>();
    uniqueRes->say_hello();
    // std::unique_ptr将在超出作用域时自动释放资源
}

// 演示std::shared_ptr的使用
void sharedPtrExample() {
    // std::shared_ptr是共享所有权的智能指针，允许多个指针指向同一资源
    std::shared_ptr<Resource> sharedRes1 = std::make_shared<Resource>();
    {
        std::shared_ptr<Resource> sharedRes2 = sharedRes1;  // 共享同一资源
        std::cout << "Shared pointer count: " << sharedRes1.use_count() << '\n';  // 打印引用计数
    }
    std::cout << "Shared pointer count: " << sharedRes1.use_count() << '\n';  // 离开作用域后，引用计数减少
    // std::shared_ptr将在最后一个指针超出作用域时自动释放资源
}

// 演示std::weak_ptr的使用
void weakPtrExample() {
    // std::weak_ptr是一种弱引用，不增加资源的引用计数
    std::shared_ptr<Resource> sharedRes = std::make_shared<Resource>();
    std::weak_ptr<Resource> weakRes = sharedRes;

    std::cout << "Shared pointer count: " << sharedRes.use_count() << '\n';  // 打印引用计数
    if (auto tempRes = weakRes.lock()) {  // 使用lock方法提升为std::shared_ptr
        tempRes->say_hello();
        std::cout << "Shared pointer count: " << sharedRes.use_count() << '\n';  // 提升后引用计数增加
    } else {
        std::cout << "Resource is no longer available\n";
    }
}

int main() {
    // 设置区域环境，确保控制台能够正确显示
    std::locale::global(std::locale(""));

    std::cout << "Unique Pointer Example:\n";
    uniquePtrExample();

    std::cout << "\nShared Pointer Example:\n";
    sharedPtrExample();

    std::cout << "\nWeak Pointer Example:\n";
    weakPtrExample();

    return 0;
}
