#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <thread>
#include <vector>

#include "libcontrol.h"
#include "rpc/RemoteManagement.h"

int main() {

    // const auto messaging_interface = std::make_unique<MessagingInterface>();

    // const auto modules = messaging_interface->find_connected_modules(std::chrono::seconds(3));

    // std::cout << "Found " << modules.size() << " modules" << std::endl;

    // for (const auto module : modules) {
    //     std::cout << "Found ID " << (int)module << std::endl;
    // }

    const auto robot_controller = std::make_shared<RobotController>();
    robot_controller->fetchDirectlyConnectedModules(true);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // const auto function_tag = 3;
    // const auto module = 99;
    // std::string msg = "Hello world!";
    // std::vector<uint8_t> parameters(msg.begin(), msg.end());
    // auto maybe_return_value = messaging_interface->remote_call(function_tag, module, parameters);

    // if (maybe_return_value) {
    //     auto return_value = std::move(*maybe_return_value);
    //     std::cout << "Got return value " << (char *)return_value->data() << std::endl;
    // } else {
    //     std::cout << "Function call time out" << std::endl;
    // }
    //

    std::string filename =
        "/Users/jslightham/Documents/Classes/capstone/firmware/build/firmware.bin";
    const auto rm = std::make_unique<RemoteManagement>(99, filename, robot_controller);
    rm->perform_ota();

    return 0;
}
