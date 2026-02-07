//
// Created by Johnathon Slightham on 2025-06-30.
//

#include "flatbuffers/AngleControlMessageBuilder.h"

namespace Flatbuffers {
const Messaging::AngleControlMessage *
AngleControlMessageBuilder::parse_angle_control_message(const uint8_t *buffer) {
  return flatbuffers::GetRoot<Messaging::AngleControlMessage>(buffer);
}

SerializedMessage
AngleControlMessageBuilder::build_angle_control_message(const int16_t angle) {
  builder_.Clear();

  const auto message = Messaging::CreateAngleControlMessage(builder_, angle);

  builder_.Finish(message);

  return {builder_.GetBufferPointer(), builder_.GetSize()};
}
} // namespace Flatbuffers
