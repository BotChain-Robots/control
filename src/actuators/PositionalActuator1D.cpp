#include "actuators/PositionalActuator1D.h"
#include "flatbuffers/SensorMessageBuilder.h"
#include "util/Variant.h"
#include <cstring>

double PositionalActuator1D::get_position() { return m_current_position; }

std::string PositionalActuator1D::get_text() { return ""; }

void PositionalActuator1D::actuate(double position) {
  m_target_position = position;
}

void PositionalActuator1D::actuate(double /* x */, double /* y */) {}

void PositionalActuator1D::actuate(const std::string &text) {}

std::vector<uint8_t> PositionalActuator1D::get_actuation_message() {
  std::vector<uint8_t> message{};

  if (m_target_position == m_board_target_position) {
    return message;
  }

  auto [data, size] =
      m_acm_builder->build_angle_control_message(m_target_position);
  message.resize(size);
  memcpy(message.data(), data, size);
  return message;
}

void PositionalActuator1D::update_sensor_data(
    const Flatbuffers::sensor_value &value) {
  std::visit(overloaded{
                 [this](Flatbuffers::target_angle a) {
                   m_board_target_position = a.angle;
                 },
                 [this](Flatbuffers::current_angle a) {
                   m_current_position = a.angle;
                 },
                 [this](Flatbuffers::current_text /*t*/) {},
             },
             value);
}
