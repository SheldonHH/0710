#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

// 前向声明 Subject 类
class Subject;

// Observer 类，模拟客户端，订阅服务器的更新
class Observer {
public:
    Observer(std::shared_ptr<Subject> subj);

    // 接收更新
    void update() {
        std::cout << "Observer updated (观察者已更新)\n";
    }

private:
    // 使用 weak_ptr 防止循环引用
    std::weak_ptr<Subject> subject; // 添加空格
};

// Subject 类，模拟服务器，管理和通知观察者
class Subject {
public:
    Subject() {
        std::cout << "Subject created, using std::vector<std::weak_ptr<Observer>> to store observers.\n";
        std::cout << "创建 Subject 对象，使用 std::vector<std::weak_ptr<Observer>> 来存储观察者。\n";
    }

    // 添加观察者到观察者列表
    void addObserver(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
        std::cout << "Observer added, using std::shared_ptr<Observer> to add observer.\n";
        std::cout << "添加观察者，使用 std::shared_ptr<Observer> 来添加观察者。\n";
    }

    // 通知所有观察者
    void notify() {
        std::cout << "Notifying observers...\n";
        std::cout << "通知所有观察者...\n";
        for (auto& obs : observers) {
            if (auto sp = obs.lock()) {
                std::cout << "Notifying an observer using std::weak_ptr<Observer>.\n";
                std::cout << "使用 std::weak_ptr<Observer> 通知观察者。\n";
                sp->update();
            } else {
                std::cout << "Skipped a null observer reference.\n";
                std::cout << "跳过一个空的观察者引用。\n";
            }
        }
    }

private:
    // 使用 weak_ptr 存储观察者，防止循环引用
    std::vector<std::weak_ptr<Observer>> observers; // 添加空格
};

// Observer 类的构造函数实现
Observer::Observer(std::shared_ptr<Subject> subj) : subject(subj) {
    std::cout << "Observer created, using std::weak_ptr<Subject> to reference the subject.\n";
    std::cout << "创建 Observer 对象，使用 std::weak_ptr<Subject> 来引用 Subject。\n";
}

// 示例函数，展示如何使用 User 和 Group 类
void socialNetworkExample() {
    std::cout << "Creating Subject...\n";
    std::cout << "创建 Subject...\n";
    // 创建服务器对象
    auto subj = std::make_shared<Subject>();

    std::cout << "Creating Observers...\n";
    std::cout << "创建 Observer...\n";
    // 创建客户端对象并订阅服务器的更新
    auto obs1 = std::make_shared<Observer>(subj);
    auto obs2 = std::make_shared<Observer>(subj);

    std::cout << "Adding Observers to Subject...\n";
    std::cout << "将 Observer 添加到 Subject...\n";
    subj->addObserver(obs1);
    subj->addObserver(obs2);

    // 服务器通知所有订阅的客户端
    subj->notify();

    // 使用 weak_ptr 防止循环引用，避免内存泄漏
    // 原因: weak_ptr 不增加引用计数，确保在 Subject 和 Observer 相互引用时不会产生循环引用。
    std::cout << "Using std::weak_ptr to prevent cyclic references and avoid memory leaks.\n";
    std::cout << "使用 std::weak_ptr 防止循环引用，避免内存泄漏。\n";
}

int main() {
    socialNetworkExample();
    return 0;
}
