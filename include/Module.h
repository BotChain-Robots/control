//
// Created by Johnathon Slightham on 2025-11-14.
//

#ifndef CONTROL_MODULE_H
#define CONTROL_MODULE_H

#include <chrono>
#include <cstdint>
#include <string>

#include "flatbuffers/SensorMessageBuilder.h"
#include "flatbuffers_generated/RobotModule_generated.h"
#include "flatbuffers_generated/TopologyMessage_generated.h"
#include "librpc.h"

struct neighbour {
    uint8_t device_id;
    Orientation orientation;
};

class Module {
  public:
    explicit Module(uint8_t device_id) : m_device_id(device_id) {};

    Module(uint8_t device_id, ModuleType module_type)
        : m_device_id(device_id), m_module_type(module_type) {};

    Module(uint8_t device_id, ModuleType module_type, Messaging::ConnectionType connection_type,
           uint8_t leader)
        : m_device_id(device_id), m_module_type(module_type), m_connection_type(connection_type),
          m_leader(leader) {};

    std::vector<neighbour> get_neighbours();

    uint8_t get_device_id();

    ModuleType get_type();

    Messaging::ConnectionType get_connection_type();

    uint8_t get_leader();

    std::chrono::time_point<std::chrono::system_clock> get_last_updated_time();

    // Not all modules implement all actuation/sensor values, some are no-ops
    virtual double get_position() = 0;
    virtual std::string get_text() = 0;
    virtual void actuate(double x) = 0;
    virtual void actuate(const std::string &text) = 0;
    virtual void actuate(double x, double y) = 0;

    void update_module_metadata(const Messaging::TopologyMessage &message);

    virtual std::vector<uint8_t> get_actuation_message() = 0;
    virtual void update_sensor_data(const Flatbuffers::sensor_value &value) = 0;

  private:
    uint8_t m_device_id;
    ModuleType m_module_type;
    Messaging::ConnectionType m_connection_type;
    uint8_t m_leader;
    std::chrono::time_point<std::chrono::system_clock> m_last_updated;
    std::vector<neighbour> m_neighbours;
    std::shared_ptr<MessagingInterface> m_messaging_interface;
};

#endif // CONTROL_MODULE_H
