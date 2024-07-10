#include "GroundStation.hpp"
#include "Satellite.hpp"

using namespace std;

GroundStation::GroundStation(shared_ptr<Satellite> sat, const string& lang) : satellite(sat), language(lang) {
    generateUUID();
    if (language == "cn") {
        cout << "创建 GroundStation 对象，UUID 为: " << uuid << "\n";
    } else {
        cout << "GroundStation created with UUID: " << uuid << "\n";
    }
}

void GroundStation::generateUUID() {
    uuid_t binuuid;
    uuid_generate(binuuid);
    char uuid_cstr[37];
    uuid_unparse(binuuid, uuid_cstr);
    uuid = uuid_cstr;
}

string GroundStation::getUUID() const {
    return uuid;
}

void GroundStation::sendData() {
    if (language == "cn") {
        cout << "地面站 " << uuid << " 正在向卫星发送数据...\n";
    } else {
        cout << "GroundStation " << uuid << " sending data to satellite...\n";
    }
    if (auto sp = satellite.lock()) {
        sp->notifyGroundStations();
    }
}

void GroundStation::receiveData() {
    if (language == "cn") {
        cout << "地面站 " << uuid << " 接收到来自卫星的数据。\n";
    } else {
        cout << "GroundStation " << uuid << " received data from satellite.\n";
    }
}
