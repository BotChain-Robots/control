cmake_minimum_required(VERSION 3.15)

project(BandwidthExample)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libcontrol REQUIRED)
find_package(spdlog REQUIRED)
find_package(librpc REQUIRED)

add_executable(BandwidthExample main.cpp)

target_link_libraries(BandwidthExample
    PRIVATE
        libcontrol::libcontrol
        spdlog::spdlog
        librpc::librpc
)
