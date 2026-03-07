#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <thread>

#include "libcontrol.h"

#define TARGET_MODULE 150
#define METADATA_TAG 7

int main() {

    const auto messaging_interface = std::make_unique<MessagingInterface>();

    const auto modules = messaging_interface->find_connected_modules(std::chrono::seconds(3));

    if (!modules.contains(TARGET_MODULE)) {
        std::cout << "Could not find target module " << TARGET_MODULE << std::endl;
        return -1;
    }

    std::vector<uint8_t> vec(1024, 'A');

    while (true) {
        messaging_interface->send(vec.data(), vec.size(), TARGET_MODULE, METADATA_TAG, false);
    }


    return 0;
}
