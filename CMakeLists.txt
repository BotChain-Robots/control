cmake_minimum_required(VERSION 3.15)
project(control LANGUAGES C CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

find_package(Eigen3 REQUIRED)
find_package(librpc REQUIRED)
find_package(flatbuffers REQUIRED)
find_package(spdlog REQUIRED)
find_package(sentry REQUIRED)
find_package(crashpad REQUIRED)

file(GLOB_RECURSE COMMON_SOURCES
        src/flatbuffers/*.cpp
        src/actuators/*.cpp
        src/Module.cpp
        src/Hub.cpp
        src/ModuleFactory.cpp
        src/libcontrol.cpp
)

# C API Library
if(WIN32)
    add_library(c_control SHARED
            src/lib_c_control.cpp
            ${COMMON_SOURCES}
            control.def
    )
    target_compile_definitions(c_control PRIVATE CONTROL_EXPORTS)
else()
    add_library(c_control SHARED
            src/lib_c_control.cpp
            ${COMMON_SOURCES}
    )
endif()

target_include_directories(c_control PUBLIC include)
target_link_libraries(c_control
    PRIVATE
        librpc::librpc
        Eigen3::Eigen
        spdlog::spdlog
        sentry-native::sentry-native
        sentry-crashpad::sentry-crashpad
)
set_property(TARGET c_control PROPERTY CXX_STANDARD 23)

install(TARGETS c_control
        EXPORT controlTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

# C++ API Library
add_library(control SHARED
        ${COMMON_SOURCES}
)

target_include_directories(control PUBLIC include)
target_link_libraries(control
    PRIVATE
        librpc::librpc
        Eigen3::Eigen
        spdlog::spdlog
        sentry-native::sentry-native
        sentry-crashpad::sentry-crashpad
)
set_property(TARGET control PROPERTY CXX_STANDARD 23)

install(TARGETS control
        EXPORT controlTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)
