//
// Created by Johnathon Slightham on 2025-11-13.
//

#ifndef CONTROL_BOUNDED1DPOSITIONALACTUATOR_H
#define CONTROL_BOUNDED1DPOSITIONALACTUATOR_H

#include <string>

#include "actuators/Actuator.h"
#include "flatbuffers/AngleControlMessageBuilder.h"
#include "flatbuffers/SensorMessageBuilder.h"

class BoundedPositionalActuator1D : public Actuator {

public:
  BoundedPositionalActuator1D(uint8_t device_id, ModuleType type,
                              double max_value, double min_value,
                              double initial_position)
      : Actuator(device_id, type), m_target_position(initial_position),
        m_max_value(max_value), m_min_value(min_value),
        acm_builder(
            std::make_unique<Flatbuffers::AngleControlMessageBuilder>()) {}

  double get_position() override;
  std::string get_text() override; // no-op
  void actuate(double position) override;
  void actuate(const std::string &text) override; // no-op
  void actuate(double x, double y) override;      // no-op

  std::vector<uint8_t> get_actuation_message() override;
  void update_sensor_data(const Flatbuffers::sensor_value &value) override;

private:
  double m_current_position = 0;
  double m_target_position;
  double m_max_value;
  double m_min_value;
  std::unique_ptr<Flatbuffers::AngleControlMessageBuilder> acm_builder;
};

#endif // CONTROL_1DPOSITIONALACTUATOR_H
