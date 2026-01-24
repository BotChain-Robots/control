
#include "ModuleFactory.h"
#include "Hub.h"
#include "actuators/BoundedPositionalActuator1D.h"
#include "actuators/PositionalActuator1D.h"
#include "flatbuffers_generated/RobotModule_generated.h"

#define SERVO1_MAX_ANGLE 180
#define SERVO1_MIN_ANGLE 0
#define SERVO1_DEFAULT_ANGLE 90

std::shared_ptr<Module>
ModuleFactory::createModule(uint8_t device_id, ModuleType type,
                            std::shared_ptr<MessagingInterface> &messaging_interface) {
    switch (type) {
    case ModuleType_SPLITTER:
        return std::make_shared<Hub>(device_id, type);
    case ModuleType_BATTERY:
        return std::make_shared<Hub>(device_id, type);
    case ModuleType_SERVO_1:
    case ModuleType_SERVO_2:
        return std::make_shared<BoundedPositionalActuator1D>(
            device_id, type, SERVO1_MAX_ANGLE, SERVO1_MIN_ANGLE, SERVO1_DEFAULT_ANGLE);
    case ModuleType_DC_MOTOR:
        return std::make_shared<PositionalActuator1D>(device_id, type);
    }
    return nullptr;
}
