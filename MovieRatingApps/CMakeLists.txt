cmake_minimum_required(VERSION 3.10)

project(MovieRatingApp)

set(CMAKE_CXX_STANDARD 14)

# 设置MySQL连接器的头文件和库文件路径（根据你的实际安装路径进行修改）
set(MYSQL_CONNECTOR_INCLUDE_DIR /usr/local/include/mysql-cppconn-8)
set(MYSQL_CONNECTOR_LIB_DIR /usr/local/lib)

# 添加MySQL连接器的头文件路径
include_directories(${MYSQL_CONNECTOR_INCLUDE_DIR})

# 添加MySQL连接器的库文件路径
link_directories(${MYSQL_CONNECTOR_LIB_DIR})

# 添加可执行文件
add_executable(MovieRatingApp main.cpp)

# 链接MySQL连接器库
target_link_libraries(MovieRatingApp mysqlcppconn8)
