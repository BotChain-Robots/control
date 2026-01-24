//
// Created by Johnathon Slightham on 2025-11-13.
//

#ifndef CONTROL_1DPOSITIONALACTUATOR_H
#define CONTROL_1DPOSITIONALACTUATOR_H

#include "actuators/Actuator.h"
#include "flatbuffers/AngleControlMessageBuilder.h"
#include "flatbuffers/SensorMessageBuilder.h"

class PositionalActuator1D final : public Actuator {

  public:
    PositionalActuator1D(uint8_t device_id, ModuleType type)
        : Actuator(device_id, type),
          m_acm_builder(std::make_unique<Flatbuffers::AngleControlMessageBuilder>()) {};

    double get_position() override;
    void actuate(double position) override;
    void actuate(double x, double y) override; // no-op

    std::vector<uint8_t> get_actuation_message() override;
    void update_sensor_data(const Flatbuffers::sensor_value &value) override;

  private:
    void update_loop();
    double m_target_position = 0;
    double m_current_position = 0;
    double m_board_target_position = 0;
    std::unique_ptr<Flatbuffers::AngleControlMessageBuilder> m_acm_builder;
};

#endif // CONTROL_1DPOSITIONALACTUATOR_H
