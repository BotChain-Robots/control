#include "Hub.h"
#include "flatbuffers/SensorMessageBuilder.h"

double Hub::get_position() {
    // no-op
    return 0;
}

std::string Hub::get_text() {
    // no-op
    return "";
}

void Hub::actuate(double /* position */) {
    // no-op
}

void Hub::actuate(double /* x */, double /* y */) {
    // no-op
}

void Hub::actuate(const std::string &text) {
    // no-op
}

std::vector<uint8_t> Hub::get_actuation_message() {
    // no-op
    return {};
}

void Hub::update_sensor_data(const Flatbuffers::sensor_value & /* value */) {
    // no-op
}
