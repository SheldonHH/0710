#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <vector>
#include <memory>
#include <mutex>
#include <string>

class GroundStation;

class Satellite {
public:
    Satellite(const std::string& lang);

    void addGroundStation(std::shared_ptr<GroundStation> gs);
    void notifyGroundStations();

private:
    std::vector<std::weak_ptr<GroundStation>> groundStations;
    std::mutex mtx;
    std::string language;
};

#endif // SATELLITE_HPP
