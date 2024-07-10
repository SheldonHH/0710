// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "CMakeProject1.h"


#include "GroundStation.hpp"
#include "Satellite.hpp"
#include <thread>
#include <iostream>

using namespace std;

void groundStationThread(shared_ptr<GroundStation> gs) {
    this_thread::sleep_for(chrono::seconds(1)); // 模拟一些延迟
    gs->sendData();
}

int main(int argc, char* argv[]) {
    string language = "en"; // 默认语言为英文
    if (argc > 1) {
        language = argv[1];
    }

    // 创建卫星对象
    auto satellite = make_shared<Satellite>(language);

    // 创建多个地面站对象并添加到卫星
    auto gs1 = make_shared<GroundStation>(satellite, language);
    auto gs2 = make_shared<GroundStation>(satellite, language);
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
