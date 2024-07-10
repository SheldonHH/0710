cmake_minimum_required(VERSION 3.10)

# 项目信息
project(SatcomProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 包含目录
include_directories(${PROJECT_SOURCE_DIR})

# 源文件列表
set(SOURCES
    main.cpp
    GroundStation.cpp
    Satellite.cpp
)

# 可执行文件
add_executable(satcom ${SOURCES})

# 链接库
target_link_libraries(satcom pthread)
