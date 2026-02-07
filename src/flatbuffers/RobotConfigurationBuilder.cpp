//
// Created by Johnathon Slightham on 2025-07-25.
//

#include "flatbuffers/RobotConfigurationBuilder.h"

#include "flatbuffers/SerializedMessage.h"
#include "flatbuffers_generated/RobotConfiguration_generated.h"

namespace Flatbuffers {

SerializedMessage RobotConfigurationBuilder::build_robot_configuration(
    const std::vector<ModuleInstance> &modules,
    const std::vector<ModuleConnectionInstance> &connections) {
  builder_.Clear();

  std::vector<flatbuffers::Offset<RobotModule>> module_vector;
  std::vector<flatbuffers::Offset<Frontend::ModuleConnection>>
      connection_vector;

  for (const auto &connection : connections) {
    connection_vector.push_back(Frontend::CreateModuleConnection(
        builder_, connection.from_module_id, connection.to_module_id,
        connection.from_socket, connection.to_socket, connection.orientation));
  }

  for (const auto &module : modules) {
    // todo: this only works for motors right now
    auto motor_state = CreateMotorState(builder_, module.angle);
    const flatbuffers::Offset<void> config_union = motor_state.Union();
    module_vector.push_back(CreateRobotModule(builder_, module.id, module.type,
                                              ModuleState_MotorState,
                                              config_union));
  }

  const auto connection_vector_fb = builder_.CreateVector(connection_vector);
  const auto module_vector_fb = builder_.CreateVector(module_vector);

  const auto message = Frontend::CreateRobotConfiguration(
      builder_, module_vector_fb, connection_vector_fb);

  builder_.Finish(message);

  return {builder_.GetBufferPointer(), builder_.GetSize()};
}

const Frontend::RobotConfiguration *
RobotConfigurationBuilder::parse_robot_configuration(
    const std::uint8_t *buffer) {
  return flatbuffers::GetRoot<Frontend::RobotConfiguration>(buffer);
}
} // namespace Flatbuffers
