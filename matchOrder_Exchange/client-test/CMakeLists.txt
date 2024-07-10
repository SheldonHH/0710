cmake_minimum_required(VERSION 3.10)
project(GTestSample)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 查找GTest库
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 添加可执行文件
add_executable(test_clients test_clients.cpp client.cpp)

# 链接GTest库
target_link_libraries(test_clients ${GTEST_LIBRARIES} pthread)
