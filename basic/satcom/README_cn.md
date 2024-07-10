# 卫星通信模拟程序

## 概述

该程序模拟了一个卫星通信系统，其中多个地面站向卫星发送和接收数据。程序展示了 C++ 智能指针（`std::shared_ptr` 和 `std::weak_ptr`）以及多线程的使用。

## 特性

- **智能指针**：程序使用 `std::shared_ptr` 管理卫星和地面站对象的生命周期，并使用 `std::weak_ptr` 避免循环依赖。
- **多线程**：程序创建多个线程来模拟地面站并发地向卫星发送数据。
- **UUID 生成**：每个地面站都分配一个唯一的 UUID 进行标识。
- **语言选择**：程序支持根据命令行参数选择中文或英文输出。

## 文件

- `GroundStation.hpp` 和 `GroundStation.cpp`：`GroundStation` 类的声明和实现。
- `Satellite.hpp` 和 `Satellite.cpp`：`Satellite` 类的声明和实现。
- `main.cpp`：包含主函数并设置模拟环境。
- `Makefile`：用于构建程序。

## 类描述

### GroundStation

`GroundStation` 类表示一个与卫星通信的地面站。

- **成员**：
  - `std::weak_ptr<Satellite> satellite`：指向卫星的弱指针，防止循环引用。
  - `std::string uuid`：地面站的唯一标识符。
  - `std::string language`：输出信息的语言。

- **方法**：
  - `GroundStation(std::shared_ptr<Satellite> sat, const std::string& lang)`：构造函数，初始化地面站并生成 UUID。
  - `void sendData()`：模拟向卫星发送数据。
  - `void receiveData()`：模拟从卫星接收数据。
  - `std::string getUUID() const`：返回地面站的 UUID。

### Satellite

`Satellite` 类表示管理和与多个地面站通信的卫星。

- **成员**：
  - `std::vector<std::weak_ptr<GroundStation>> groundStations`：地面站的弱指针向量。
  - `std::mutex mtx`：保护地面站列表访问的互斥锁。
  - `std::string language`：输出信息的语言。

- **方法**：
  - `Satellite(const std::string& lang)`：构造函数，初始化卫星。
  - `void addGroundStation(std::shared_ptr<GroundStation> gs)`：向卫星的列表中添加地面站。
  - `void notifyGroundStations()`：通知所有地面站。

## 使用

### 构建程序

使用提供的 `Makefile` 构建程序。在终端中运行以下命令：

```bash
graph TD
    A[启动程序] --> B[创建 Satellite 对象]
    B --> C[创建 GroundStation 对象并生成 UUID]
    C --> D[添加 GroundStation 到 Satellite]
    D --> E[创建并启动 GroundStation 线程]
    E --> F[GroundStation 向 Satellite 发送数据]
    F --> G[Satellite 通知所有 GroundStations]
    G --> H[GroundStation 接收数据]

```