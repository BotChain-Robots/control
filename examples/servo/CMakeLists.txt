cmake_minimum_required(VERSION 3.15)

project(ServoExample)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libcontrol REQUIRED)
find_package(spdlog REQUIRED)
find_package(librpc REQUIRED)

add_executable(ServoExample main.cpp)

target_link_libraries(ServoExample
    PRIVATE
        libcontrol::libcontrol
        spdlog::spdlog
        librpc::librpc
)
