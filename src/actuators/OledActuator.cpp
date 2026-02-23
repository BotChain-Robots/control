#include "actuators/OledActuator.h"
#include "flatbuffers/SensorMessageBuilder.h"
#include "util/Variant.h"
#include <cstring>

std::string OledActuator::get_text() {
    return m_current_text;
}

void OledActuator::actuate(const std::string &text) {
    m_target_text = text;
}

std::vector<uint8_t> OledActuator::get_actuation_message() {
    std::vector<uint8_t> message{};

    if (m_target_text == m_current_text) {
        return message;
    }

    auto [data, size] = m_text_message_builder->build_text_control_message(m_target_text);
    message.resize(size);
    memcpy(message.data(), data, size);
    return message;
}

void OledActuator::update_sensor_data(const Flatbuffers::sensor_value &value) {
    std::visit(overloaded{
                   [this](Flatbuffers::target_angle /* a */) {},
                   [this](Flatbuffers::current_angle /* a */) {},
                   [this](Flatbuffers::current_text t) { m_current_text = t.text; },
               },
               value);
}
