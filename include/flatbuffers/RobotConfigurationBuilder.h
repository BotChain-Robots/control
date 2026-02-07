//
// Created by Johnathon Slightham on 2025-07-25.
//

#ifndef ROBOTCONFIGURATIONBUILDER_H
#define ROBOTCONFIGURATIONBUILDER_H
#include "SerializedMessage.h"
#include <cstdint>
#include <flatbuffers_generated/RobotConfiguration_generated.h>
#include <flatbuffers_generated/RobotModule_generated.h>
#include <vector>

namespace Flatbuffers {
struct ModuleInstance {
  uint8_t id;
  ModuleType type;
  int angle;
};

struct ModuleConnectionInstance {
  uint8_t from_module_id;
  uint8_t to_module_id;
  uint8_t from_socket;
  uint8_t to_socket;
  Orientation orientation;
};

class RobotConfigurationBuilder {
public:
  RobotConfigurationBuilder() : builder_(1024) {}

  SerializedMessage build_robot_configuration(
      const std::vector<ModuleInstance> &modules,
      const std::vector<ModuleConnectionInstance> &connections);

  static const Frontend::RobotConfiguration *
  parse_robot_configuration(const std::uint8_t *buffer);

private:
  flatbuffers::FlatBufferBuilder builder_;
};
} // namespace Flatbuffers

#endif // ROBOTCONFIGURATIONBUILDER_H
