//
// Created by Johnathon Slightham on 2025-06-30.
//

#include "flatbuffers/TextControlMessageBuilder.h"

namespace Flatbuffers {

SerializedMessage
TextControlMessageBuilder::build_text_control_message(std::string &t) {
  builder_.Clear();

  auto text_offset = builder_.CreateString(t);
  const auto message =
      Messaging::CreateTextControlMessage(builder_, text_offset);

  builder_.Finish(message);

  return {builder_.GetBufferPointer(), builder_.GetSize()};
}
} // namespace Flatbuffers
