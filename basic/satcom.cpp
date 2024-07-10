#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

// 前向声明 Satellite 类
class Satellite;

// GroundStation 类，模拟地面站，发送数据到卫星
class GroundStation : public enable_shared_from_this<GroundStation> {
public:
    GroundStation(shared_ptr<Satellite> sat);

    void sendData() {
        cout << "GroundStation sending data to satellite...\n";
        cout << "地面站正在向卫星发送数据...\n";
        if (auto sp = satellite.lock()) {
            sp->notifyGroundStations();
        }
    }

    void receiveData() {
        cout << "GroundStation received data from satellite.\n";
        cout << "地面站接收到来自卫星的数据。\n";
    }

private:
    // 使用 weak_ptr 防止循环引用
    weak_ptr<Satellite> satellite;
};

// Satellite 类，模拟卫星，接收和处理地面站的数据
class Satellite {
public:
    Satellite() {
        cout << "Satellite created, using std::vector<std::weak_ptr<GroundStation>> to store ground stations.\n";
        cout << "创建 Satellite 对象，使用 std::vector<std::weak_ptr<GroundStation>> 来存储地面站。\n";
    }

    // 添加地面站到地面站列表
    void addGroundStation(shared_ptr<GroundStation> gs) {
        lock_guard<mutex> lock(mtx);
        groundStations.push_back(gs);
        cout << "Ground station added, using std::shared_ptr<GroundStation> to add ground station.\n";
        cout << "添加地面站，使用 std::shared_ptr<GroundStation> 来添加地面站。\n";
    }

    // 通知所有地面站
    void notifyGroundStations() {
        lock_guard<mutex> lock(mtx);
        cout << "Notifying ground stations...\n";
        cout << "通知所有地面站...\n";
        for (auto& gs : groundStations) {
            if (auto sp = gs.lock()) {
                cout << "Notifying a ground station using std::weak_ptr<GroundStation>.\n";
                cout << "使用 std::weak_ptr<GroundStation> 通知地面站。\n";
                sp->receiveData();
            } else {
                cout << "Skipped a null ground station reference.\n";
                cout << "跳过一个空的地面站引用。\n";
            }
        }
    }

private:
    // 使用 weak_ptr 存储地面站，防止循环引用
    vector<weak_ptr<GroundStation>> groundStations;
    mutex mtx; // 用于保护地面站列表的互斥锁
};

// GroundStation 构造函数的实现
GroundStation::GroundStation(shared_ptr<Satellite> sat) : satellite(sat) {
    cout << "GroundStation created, using std::weak_ptr<Satellite> to reference the satellite.\n";
    cout << "创建 GroundStation 对象，使用 std::weak_ptr<Satellite> 来引用 Satellite。\n";
}

void groundStationThread(shared_ptr<GroundStation> gs) {
    this_thread::sleep_for(chrono::seconds(1)); // 模拟一些延迟
    gs->sendData();
}

int main() {
    // 创建卫星对象
    auto satellite = make_shared<Satellite>();

    // 创建多个地面站对象并添加到卫星
    auto gs1 = make_shared<GroundStation>(satellite);
    auto gs2 = make_shared<GroundStation>(satellite);
    satellite->addGroundStation(gs1);
    satellite->addGroundStation(gs2);

    // 创建并启动地面站线程
    thread t1(groundStationThread, gs1);
    thread t2(groundStationThread, gs2);

    // 等待线程完成
    t1.join();
    t2.join();

    return 0;
}
