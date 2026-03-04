#ifndef CONTROL_DISTANCESENSOR_H
#define CONTROL_DISTANCESENSOR_H

#include "Module.h"
#include "flatbuffers/SensorMessageBuilder.h"
#include "util/Variant.h"

class DistanceSensor : public Module {

  public:
    DistanceSensor(uint8_t device_id, ModuleType type) : Module(device_id, type) {
    }

    double get_distance() override;

    std::vector<uint8_t> get_actuation_message() override;
    void update_sensor_data(const Flatbuffers::sensor_value &value) override;

  private:
    double m_current_distance = 0.0;
};

#endif // CONTROL_DISTANCESENSOR_H
