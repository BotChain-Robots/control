//
// Created by Johnathon Slightham on 2025-06-30.
//

#ifndef ANGLECONTROLMESSAGEBUILDER_H_
#define ANGLECONTROLMESSAGEBUILDER_H_

#include <string>
#include <vector>

#include "SerializedMessage.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers_generated/AngleControlMessage_generated.h"

namespace Flatbuffers {
class AngleControlMessageBuilder {
public:
  AngleControlMessageBuilder() : builder_(256) {}

  SerializedMessage build_angle_control_message(int16_t angle);
  static const Messaging::AngleControlMessage *
  parse_angle_control_message(const uint8_t *buffer);

private:
  flatbuffers::FlatBufferBuilder builder_;
};
} // namespace Flatbuffers

#endif
