#ifndef CONTROL_MODULEFACTORY_H
#define CONTROL_MODULEFACTORY_H

#include "Module.h"
#include "flatbuffers_generated/RobotModule_generated.h"

class ModuleFactory {
  public:
    static std::shared_ptr<Module>
    createModule(uint8_t device_id, ModuleType type,
                 std::shared_ptr<MessagingInterface> &messaging_interface);
};

#endif // CONTROL_MODULEFACTORY_H
