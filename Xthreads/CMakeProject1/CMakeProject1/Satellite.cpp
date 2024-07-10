#include "Satellite.hpp"
#include "GroundStation.hpp"

using namespace std;

Satellite::Satellite(const string& lang) : language(lang) {
    if (language == "cn") {
        cout << "创建 Satellite 对象，使用 std::vector<std::weak_ptr<GroundStation>> 来存储地面站。\n";
    } else {
        cout << "Satellite created, using std::vector<std::weak_ptr<GroundStation>> to store ground stations.\n";
    }
}

void Satellite::addGroundStation(shared_ptr<GroundStation> gs) {
    lock_guard<mutex> lock(mtx);
    groundStations.push_back(gs);
    if (language == "cn") {
        cout << "UUID 为: " << gs->getUUID() << " 的地面站已添加。\n";
    } else {
        cout << "Ground station with UUID: " << gs->getUUID() << " added.\n";
    }
}

void Satellite::notifyGroundStations() {
    lock_guard<mutex> lock(mtx);
    if (language == "cn") {
        cout << "通知所有地面站...\n";
    } else {
        cout << "Notifying ground stations...\n";
    }
    for (auto& gs : groundStations) {
        if (auto sp = gs.lock()) {
            if (language == "cn") {
                cout << "通知 UUID 为: " << sp->getUUID() << " 的地面站。\n";
            } else {
                cout << "Notifying ground station with UUID: " << sp->getUUID() << ".\n";
            }
            sp->receiveData();
        } else {
            if (language == "cn") {
                cout << "跳过一个空的地面站引用。\n";
            } else {
                cout << "Skipped a null ground station reference.\n";
            }
        }
    }
}
