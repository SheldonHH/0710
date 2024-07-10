# Satcom Simulation Program

## Overview

This program simulates a satellite communication system where multiple ground stations send and receive data to and from a satellite. The program demonstrates the usage of C++ smart pointers (`std::shared_ptr` and `std::weak_ptr`) and multi-threading.

## Features

- **Smart Pointers**: The program uses `std::shared_ptr` to manage the lifecycle of the satellite and ground station objects, and `std::weak_ptr` to avoid cyclic dependencies.
- **Multi-threading**: The program creates multiple threads to simulate ground stations sending data to the satellite concurrently.
- **UUID Generation**: Each ground station is assigned a unique UUID for identification.
- **Language Selection**: The program supports English and Chinese outputs based on command-line arguments.

## Files

- `GroundStation.hpp` and `GroundStation.cpp`: Declaration and implementation of the `GroundStation` class.
- `Satellite.hpp` and `Satellite.cpp`: Declaration and implementation of the `Satellite` class.
- `main.cpp`: Contains the main function and sets up the simulation.
- `Makefile`: Used to build the program.

## Class Descriptions

### GroundStation

The `GroundStation` class represents a ground station that communicates with the satellite.

- **Members**:
  - `std::weak_ptr<Satellite> satellite`: A weak pointer to the satellite to prevent cyclic references.
  - `std::string uuid`: A unique identifier for the ground station.
  - `std::string language`: The language for output messages.

- **Methods**:
  - `GroundStation(std::shared_ptr<Satellite> sat, const std::string& lang)`: Constructor that initializes the ground station and generates a UUID.
  - `void sendData()`: Simulates sending data to the satellite.
  - `void receiveData()`: Simulates receiving data from the satellite.
  - `std::string getUUID() const`: Returns the UUID of the ground station.

### Satellite

The `Satellite` class represents a satellite that manages and communicates with multiple ground stations.

- **Members**:
  - `std::vector<std::weak_ptr<GroundStation>> groundStations`: A vector of weak pointers to ground stations.
  - `std::mutex mtx`: A mutex to protect access to the ground station list.
  - `std::string language`: The language for output messages.

- **Methods**:
  - `Satellite(const std::string& lang)`: Constructor that initializes the satellite.
  - `void addGroundStation(std::shared_ptr<GroundStation> gs)`: Adds a ground station to the satellite's list.
  - `void notifyGroundStations()`: Notifies all ground stations.

## Usage

### Building the Program

To build the program, use the provided `Makefile`. Run the following command in the terminal:


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