#include "flatbuffers/SensorMessageBuilder.h"
#include "flatbuffers_generated/SensorMessage_generated.h"

namespace Flatbuffers {

const Messaging::SensorMessage *SensorMessageBuilder::parse_sensor_message(const uint8_t *buffer) {
    return flatbuffers::GetRoot<Messaging::SensorMessage>(buffer);
}

} // namespace Flatbuffers
