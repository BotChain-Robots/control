#include <chrono>
#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "libcontrol.h"
#include "rpc/RemoteManagement.h"

void progress_monitor_task(std::shared_ptr<RemoteManagement> rm, uint8_t module_id) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(10, 100);

    std::cout << dist6(rng) << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000 + dist6(rng)));
        std::cout << "Module " << (int)module_id << ": " << rm->ota_progress() * 100 << "% complete"
                  << std::endl;
        if (rm->ota_progress() >= 1.0)
            return;
    }
}

void task(std::shared_ptr<RobotController> controller, uint8_t module_id,
          const std::string &filepath) {
    auto rm = std::make_shared<RemoteManagement>(module_id, filepath, controller);
    std::thread t(progress_monitor_task, rm, module_id);
    rm->perform_ota();
    t.join();
    std::cout << "Done updating " << (int)module_id << std::endl;
}

int main() {
    const auto robot_controller = std::make_shared<RobotController>();
    robot_controller->fetchDirectlyConnectedModules(true);
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::vector<uint8_t> to_update{};
    for (const auto &maybe_module : robot_controller->getModules()) {
        if (const auto &module = maybe_module.lock()) {
            if (module->get_leader() != module->get_device_id()) {
                continue;
            }
            std::cout << "Updating module " << (int)module->get_device_id() << std::endl;
            to_update.emplace_back(module->get_device_id());
        }
    }

    if (to_update.size() < 1) {
        std::cout << "No modules found to update" << std::endl;
        return 0;
    }

    std::string filename = "/home/jslightham/Documents/capstone/firmware/build/firmware.bin";
    std::vector<std::thread> threads;
    for (int i = 0; i < to_update.size(); i++) {
        threads.emplace_back(task, robot_controller, to_update[i], filename);
    }

    for (auto &t : threads) {
        t.join();
    }

    return 0;
}
