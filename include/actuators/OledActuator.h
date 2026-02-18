//
// Created by Johnathon Slightham on 2025-11-13.
//

#ifndef CONTROL_OLEDACTUATOR_H
#define CONTROL_OLEDACTUATOR_H

#include <string>

#include "actuators/Actuator.h"
#include "flatbuffers/SensorMessageBuilder.h"
#include "flatbuffers/TextControlMessageBuilder.h"

class OledActuator : public Actuator {

  public:
    OledActuator(uint8_t device_id, ModuleType type)
        : Actuator(device_id, type),
          m_text_message_builder(std::make_unique<Flatbuffers::TextControlMessageBuilder>()) {
    }

    double get_position() override;
    std::string get_text() override;
    void actuate(double position) override;
    void actuate(double x, double y) override; // no-op
    void actuate(const std::string &text) override;

    std::vector<uint8_t> get_actuation_message() override;
    void update_sensor_data(const Flatbuffers::sensor_value &value) override;

  private:
    std::string m_current_text = "";
    std::string m_target_text = "";
    std::unique_ptr<Flatbuffers::TextControlMessageBuilder> m_text_message_builder;
};

#endif // CONTROL_OLEDACTUATOR_H
