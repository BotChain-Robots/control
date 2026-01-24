#include "Hub.h"
#include "flatbuffers/SensorMessageBuilder.h"

double Hub::get_position() {
    // no-op
    return 0;
}

void Hub::actuate(double /* position */) {
    // no-op
}

void Hub::actuate(double /* x */, double /* y */) {
    // no-op
}

std::vector<uint8_t> Hub::get_actuation_message() {
    // no-op
    return {};
}

void Hub::update_sensor_data(const Flatbuffers::sensor_value & /* value */) {
    // no-op
}
