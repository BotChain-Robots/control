

#ifndef OTAPACKETBUILDER_H
#define OTAPACKETBUILDER_H

#include <vector>

#include "flatbuffers/SerializedMessage.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers_generated/OTAPacket_generated.h"

namespace Flatbuffers {

class OTAPacketBuilder {
  public:
    OTAPacketBuilder() : builder_(1024) {
    }

    SerializedMessage build_ota_packet(uint16_t packet_num, const std::vector<uint8_t> &packet);

    static const Messaging::OTAPacket *parse_ota_packet(const uint8_t *buffer);

  private:
    flatbuffers::FlatBufferBuilder builder_;
};
} // namespace Flatbuffers

#endif // OTAPACKETBUILDER_H
