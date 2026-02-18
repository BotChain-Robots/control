//
// Created by Johnathon Slightham on 2025-06-30.
//

#ifndef TEXTCONTROLMESSAGEBUILDER_H_
#define TEXTCONTROLMESSAGEBUILDER_H_

#include <string>
#include <vector>

#include "SerializedMessage.h"
#include "flatbuffers/flatbuffers.h"
#include "flatbuffers_generated/TextControlMessage_generated.h"

namespace Flatbuffers {
class TextControlMessageBuilder {
  public:
    TextControlMessageBuilder() : builder_(256) {
    }

    SerializedMessage build_text_control_message(std::string &t);

  private:
    flatbuffers::FlatBufferBuilder builder_;
};
} // namespace Flatbuffers

#endif
