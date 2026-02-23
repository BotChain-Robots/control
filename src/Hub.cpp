#include "Hub.h"
#include "flatbuffers/SensorMessageBuilder.h"

std::vector<uint8_t> Hub::get_actuation_message() {
    // no-op
    return {};
}

void Hub::update_sensor_data(const Flatbuffers::sensor_value & /* value */) {
    // no-op
}
