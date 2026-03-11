#include <chrono>
#include <format>
#include <iostream>
#include <random>
#include <thread>

#include "libcontrol.h"

int main() {

    const auto controller = std::make_unique<RobotController>();

    controller->fetchDirectlyConnectedModules(true);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Found " << controller->getModules().size() << " modules" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 10);

    while (true) {
        std::cout << "Found " << controller->getModules().size() << " modules" << std::endl;
        for (const auto &maybe_module : controller->getModules()) {
            if (const auto &module = maybe_module.lock()) {
                if (module->get_type() == ModuleType_SERVO_2 ||
                    module->get_type() == ModuleType_SERVO_1) {
                    std::cout << "Actuating " << (int)module->get_device_id() << " leader "
                              << (int)module->get_leader() << std::endl;
                    int randomNumber = dist(gen);
                    if (module->get_position() > 90) {
                        module->actuate(50 - randomNumber);
                    } else {
                        module->actuate(150 + randomNumber);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
            }
        }
    }

    return 0;
}
