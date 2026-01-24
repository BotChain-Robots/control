//
// Created by Johnathon Slightham on 2025-06-30.
//

#ifndef SENSORMESSAGEBUILDER_H
#define SENSORMESSAGEBUILDER_H

#include <variant>

#include "flatbuffers_generated/SensorMessage_generated.h"

namespace Flatbuffers {

struct target_angle {
    int16_t angle;
};

struct current_angle {
    int16_t angle;
};

typedef std::variant<target_angle, current_angle> sensor_value;

class SensorMessageBuilder {
  public:
    SensorMessageBuilder() : builder_(1024) {
    }

    static const Messaging::SensorMessage *parse_sensor_message(const std::uint8_t *buffer);

    template <typename T>
    static std::optional<sensor_value> build_sensor_value(Messaging::SensorValue type, T value) {
        switch (type) {
        case Messaging::SensorValue_TargetAngle: {
            const Messaging::TargetAngle *target =
                static_cast<const Messaging::TargetAngle *>(value);
            return target_angle{target->value()};
        }
        case Messaging::SensorValue_CurrentAngle: {
            const Messaging::CurrentAngle *current =
                static_cast<const Messaging::CurrentAngle *>(value);
            return current_angle{current->value()};
        }
        default:
            return std::nullopt;
        }
    }

  private:
    flatbuffers::FlatBufferBuilder builder_;
};
} // namespace Flatbuffers

#endif // TOPOLOGYMESSAGEBUILDER_H
