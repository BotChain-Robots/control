
#include "flatbuffers/OTAPacketBuilder.h"
#include "flatbuffers/SerializedMessage.h"

namespace Flatbuffers {

SerializedMessage OTAPacketBuilder::build_ota_packet(uint16_t packet_num,
                                                     const std::vector<uint8_t> &packet) {
    builder_.Clear();

    const auto packet_vector = builder_.CreateVector(packet);

    const auto message = Messaging::CreateOTAPacket(builder_, packet_num,
                                                    static_cast<int>(packet.size()), packet_vector);

    builder_.Finish(message);

    return {builder_.GetBufferPointer(), builder_.GetSize()};
}

const Messaging::OTAPacket *OTAPacketBuilder::parse_ota_packet(const uint8_t *buffer) {
    return flatbuffers::GetRoot<Messaging::OTAPacket>(buffer);
}

} // namespace Flatbuffers
