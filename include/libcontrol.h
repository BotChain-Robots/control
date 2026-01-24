//
// Created by Johnathon Slightham on 2025-11-13.
//

#ifndef CONTROL_LIBCONTROL_H
#define CONTROL_LIBCONTROL_H

#include <memory>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include "Module.h"
#include "flatbuffers/RobotConfigurationBuilder.h"
#include "librpc.h"

/**
 * \brief The root class of the BotChain control library.
 *
 * This class finds and connects to BotChain modules accessible to the PC.
 * Providing an interface to control actuator modules, and get sensor readings
 * from sensor modules.
 */
class RobotController {

  public:
    /**
   * \brief Creates a new RobotController.
   *
   * Each RobotController will establish unique connections to accessible
   * BotChain modules.
   */
    RobotController()
        : m_messaging_interface(std::make_unique<MessagingInterface>()),
          m_metadata_loop(std::thread(&RobotController::metadata_loop, this)),
          m_transmit_loop(std::thread(&RobotController::transmit_loop, this)),
          m_configuration_loop(std::thread(&RobotController::configuration_loop, this)),
          m_sensor_loop(std::thread(&RobotController::sensor_loop, this)),
          m_expiry_looop(std::thread(&RobotController::expiry_loop, this)) {
        m_logger = spdlog::basic_logger_mt("default_logger", "libcontrol.log");
        spdlog::flush_on(spdlog::level::info);
        spdlog::set_default_logger(m_logger);
    }

    ~RobotController();

    /**
   * \brief Get a list of accessible modules
   *
   * Returns a std::vector containing robotic modules. This list includes
   * modules connected directly to the PC, as well as modules connected
   * through eachother. Sensor readings can be obtained, and actuation
   * commands can be sent by calling the "as" function for the correct
   * type (can be identified through the ModuleType).
   */
    std::vector<std::weak_ptr<Module>> getModules();

    /**
   * \brief Get a module by ID
   *
   * Returns a std::vector containing robotic modules. This list includes
   * modules connected directly to the PC, as well as modules connected
   * through eachother. Sensor readings can be obtained, and actuation
   * commands can be sent by calling the "as" function for the correct
   * type (can be identified through the ModuleType).
   */
    std::optional<std::weak_ptr<Module>> getModule(uint8_t device_id);

    /**
   * \brief Get a list of all connections.
   *
   * Returns a list containing all connections between modules.
   */
    std::vector<Flatbuffers::ModuleConnectionInstance> getConnections();

    /**
   * \brief Get a list of accessible modules.
   *
   * Returns a list containing ID and types of each module.
   */
    std::vector<Flatbuffers::ModuleInstance> getModuleList();

    /**
   * \brief Reset the list of modules.
   *
   * Reset the internal list containing known modules. Note: This list is
   * automatically updated, only call this function when you need to update the
   * list faster than the internal refresh logic.
   */
    void resetModules();

    /**
   * \brief Poll for devices accessible to the PC.
   *
   * Manually trigger a poll for devices accessible to the PC.
   */
    void fetchDirectlyConnectedModules(bool block);

  private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::unordered_map<uint8_t, std::shared_ptr<Module>> m_id_to_module{};
    std::unordered_map<uint8_t, std::vector<Flatbuffers::ModuleConnectionInstance>>
        m_connection_map{};
    std::shared_mutex m_module_lock{};
    std::shared_mutex m_connection_lock{};
    std::shared_ptr<MessagingInterface> m_messaging_interface;
    std::atomic<bool> m_stop_thread{false}; // todo: make sure threads stop if we
                                            // dont get any messages (timeouts)
    std::thread m_metadata_loop;
    std::thread m_transmit_loop;
    std::thread m_configuration_loop;
    std::thread m_sensor_loop;
    std::thread m_expiry_looop;

    void metadata_loop();
    void transmit_loop();
    void configuration_loop();
    void sensor_loop();
    void expiry_loop();
};

#endif // CONTROL_LIBCONTROL_H
