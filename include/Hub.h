//
// Created by Johnathon Slightham on 2026-01-13.
//

#ifndef CONTROL_HUB_H
#define CONTROL_HUB_H

#include "Module.h"
#include "flatbuffers/SensorMessageBuilder.h"

class Hub final : public Module {

  public:
    explicit Hub(uint8_t device_id) : Module(device_id) {};

    Hub(uint8_t device_id, ModuleType module_type) : Module(device_id, module_type) {};

    Hub(uint8_t device_id, ModuleType module_type, Messaging::ConnectionType connection_type,
        uint8_t leader)
        : Module(device_id, module_type, connection_type, leader) {};

    double get_position() override;
    void actuate(double position) override;
    void actuate(double x, double y) override;
    std::vector<uint8_t> get_actuation_message() override;
    void update_sensor_data(const Flatbuffers::sensor_value &value) override;
};

#endif // CONTROL_HUB_H
