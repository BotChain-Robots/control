
#ifndef CONTROL_ACTUATOR_H
#define CONTROL_ACTUATOR_H

#include <cstdint>

#include "Module.h"
#include "flatbuffers_generated/RobotModule_generated.h"
#include "flatbuffers_generated/TopologyMessage_generated.h"

class Actuator : public Module {
public:
  explicit Actuator(uint8_t device_id) : Module(device_id) {};

  Actuator(uint8_t device_id, ModuleType module_type)
      : Module(device_id, module_type) {};

  Actuator(uint8_t device_id, ModuleType module_type,
           Messaging::ConnectionType connection_type, uint8_t leader)
      : Module(device_id, module_type, connection_type, leader) {};

protected:
  virtual std::vector<uint8_t> get_actuation_message() = 0;

private:
  [[noreturn]] void actuator_tx_loop();
};

#endif // CONTROL_MODULE_H
