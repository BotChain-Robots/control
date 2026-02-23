
#include <cstring>
#include <variant>

#include "actuators/BoundedPositionalActuator1D.h"
#include "flatbuffers/SensorMessageBuilder.h"
#include "util/Variant.h"

double BoundedPositionalActuator1D::get_position() {
    return m_current_position;
}

void BoundedPositionalActuator1D::actuate(double position) {
    if (position < m_min_value || position > m_max_value) {
        return;
    }

    m_target_position = position;
}

std::vector<uint8_t> BoundedPositionalActuator1D::get_actuation_message() {
    std::vector<uint8_t> message{};

    if (m_target_position == m_current_position) {
        return message;
    }

    auto [data, size] = acm_builder->build_angle_control_message(m_target_position);
    message.resize(size);
    memcpy(message.data(), data, size);
    return message;
}

void BoundedPositionalActuator1D::update_sensor_data(const Flatbuffers::sensor_value &value) {
    std::visit(overloaded{
                   [this](Flatbuffers::target_angle a) { m_current_position = a.angle; },
                   [this](Flatbuffers::current_angle a) { m_current_position = a.angle; },
                   [this](Flatbuffers::current_text /*t*/) {},
               },
               value);
}
