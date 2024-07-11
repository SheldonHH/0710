cmake_minimum_required(VERSION 3.10)

# 项目名称
project(MyProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含路径
include_directories(/opt/homebrew/include)

# 链接库路径
link_directories(/opt/homebrew/lib)

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS system)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Boost not found")
endif()

# 添加可执行文件
add_executable(client client.cpp)
add_executable(server server.cpp)
add_executable(batch_clients_single batch_tests/batch_clients_single.cpp)
add_executable(batch_clients_future_async batch_tests/batch_clients_future_async.cpp)
add_executable(batch_clients_threadPool batch_tests/batch_clients_threadPool.cpp)


# 链接Boost库
target_link_libraries(client Boost::system)
target_link_libraries(server Boost::system)
