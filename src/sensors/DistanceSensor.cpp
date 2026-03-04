
#include "sensors/DistanceSensor.h"

double DistanceSensor::get_distance() {
    return m_current_distance;
}

std::vector<uint8_t> DistanceSensor::get_actuation_message() {
    return {};
}

void DistanceSensor::update_sensor_data(const Flatbuffers::sensor_value &value) {
    std::visit(overloaded{[this](Flatbuffers::target_angle /*a*/) {},
                          [this](Flatbuffers::current_angle /*a*/) {},
                          [this](Flatbuffers::current_text /*t*/) {},
                          [this](Flatbuffers::distance d) { m_current_distance = d.distance; }},
               value);
}
