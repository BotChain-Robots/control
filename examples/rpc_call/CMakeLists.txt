cmake_minimum_required(VERSION 3.15)

project(RpcCallExample)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libcontrol REQUIRED)
find_package(spdlog REQUIRED)
find_package(librpc REQUIRED)

add_executable(RpcCallExample main.cpp flatbuffers/OTAPacketBuilder.cpp rpc/RemoteDebugging.cpp rpc/RemoteManagement.cpp)

target_include_directories(RpcCallExample PUBLIC include)

target_link_libraries(RpcCallExample
    PRIVATE
        libcontrol::libcontrol
        spdlog::spdlog
        librpc::librpc
)
