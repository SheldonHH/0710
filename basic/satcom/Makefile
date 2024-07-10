# 变量定义
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lpthread
TARGET = satcom
OBJECTS = main.o GroundStation.o Satellite.o

# 目标规则
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# 依赖规则
main.o: main.cpp GroundStation.hpp Satellite.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

GroundStation.o: GroundStation.cpp GroundStation.hpp Satellite.hpp
	$(CXX) $(CXXFLAGS) -c GroundStation.cpp

Satellite.o: Satellite.cpp Satellite.hpp GroundStation.hpp
	$(CXX) $(CXXFLAGS) -c Satellite.cpp

# 清理规则
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
