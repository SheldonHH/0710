#ifndef GROUNDSTATION_HPP
#define GROUNDSTATION_HPP

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <uuid/uuid.h>

class Satellite;

class GroundStation : public std::enable_shared_from_this<GroundStation> {
    // GroundStation 类继承自 std::enable_shared_from_this<GroundStation>
    // 以便在类方法中安全地创建 std::shared_ptr<GroundStation>
    // 原因：是为了在类的方法中获取指向当前对象的 std::shared_ptr
    //       比如在回调函数中传递自身的 shared_ptr，而不是直接使用 this 指针
public:
    GroundStation(std::shared_ptr<Satellite> sat, const std::string& lang);
    // 构造函数，接受一个 std::shared_ptr<Satellite> 对象和一个表示语言的字符串
    // 原因：是为了初始化地面站对象，并通过 shared_ptr 与 Satellite 对象建立关联
    //       同时设置输出信息的语言环境（中文或英文）


    void sendData();
    void receiveData();
    std::string getUUID() const;

private:
    std::weak_ptr<Satellite> satellite;
        // 使用 std::weak_ptr<Satellite> 来引用 Satellite 对象
    // 原因：避免循环引用导致的内存泄漏问题
    //       GroundStation 对象通过 shared_ptr 拥有 Satellite 对象的一部分所有权
    //       如果 Satellite 对象也通过 shared_ptr 拥有 GroundStation 对象，将会导致循环引用
    //       使用 weak_ptr 可以打破这种循环引用，因为 weak_ptr 不会增加引用计数
    std::string uuid;
    std::string language;
    void generateUUID();
};

#endif // GROUNDSTATION_HPP
