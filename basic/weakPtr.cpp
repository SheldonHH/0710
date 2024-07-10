#include <iostream>
#include <memory>
#include <vector>

// 前向声明 Observer 类
class Observer;

// Subject 类，模拟服务器，管理和通知观察者
class Subject {
public:
    // 添加观察者到观察者列表
    void addObserver(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }

    // 通知所有观察者
    void notify() {
        for (auto& obs : observers) {
            if (auto sp = obs.lock()) {
                sp->update();
            }
        }
    }

private:
    // 使用 weak_ptr 存储观察者，防止循环引用
    std::vector<std::weak_ptr<Observer>> observers;
};

// Observer 类，模拟客户端，订阅服务器的更新
class Observer {
public:
    Observer(std::shared_ptr<Subject> subj) : subject(subj) {}

    // 接收更新
    void update() {
        std::cout << "Observer updated\n";
    }

private:
    // 使用 weak_ptr 防止循环引用
    std::weak_ptr<Subject> subject;
};

// 演示观察者模式的函数
void observerPattern() {
    // 创建服务器对象
    std::shared_ptr<Subject> subj = std::make_shared<Subject>();

    // 创建客户端对象并订阅服务器的更新
    std::shared_ptr<Observer> obs1 = std::make_shared<Observer>(subj);
    std::shared_ptr<Observer> obs2 = std::make_shared<Observer>(subj);

    subj->addObserver(obs1);
    subj->addObserver(obs2);

    // 服务器通知所有订阅的客户端
    subj->notify();
    // 使用 weak_ptr 防止循环引用，避免内存泄漏
    // 原因: weak_ptr 不增加引用计数，确保在 Subject 和 Observer 相互引用时不会产生循环引用。
}

int main() {
    observerPattern();
    return 0;
}
