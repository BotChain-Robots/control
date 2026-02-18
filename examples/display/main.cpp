#include <chrono>
#include <format>
#include <iostream>
#include <thread>

#include "libcontrol.h"

int main() {

    const auto controller = std::make_unique<RobotController>();

    controller->fetchDirectlyConnectedModules(true);

    std::this_thread::sleep_for(std::chrono::seconds(1));


    std::cout << "Found " << controller->getModules().size() << " modules" << std::endl;

    for (const auto &maybe_module : controller->getModules()) {
        if (const auto &module = maybe_module.lock()) {
            std::cout << "Found module " << (int)module->get_device_id();

            if (module->get_type() == ModuleType_DISPLAY) {
                module->actuate(
                    std::format("BotChain \n\n\nModule ID: {}", module->get_device_id()));
            }
        }
    }

    return 0;
}
