//
// Created by Johnathon Slightham on 2026-02-16.
//

#ifndef REMOTEMANAGEMENT_H
#define REMOTEMANAGEMENT_H

#include <cstdint>
#include <fstream>
#include <memory>

#include "flatbuffers/OTAPacketBuilder.h"
#include "libcontrol.h"

class RemoteManagement {
  public:
    RemoteManagement(uint8_t module_id, const std::string &path,
                     std::shared_ptr<RobotController> controller)
        : m_module_id(module_id), m_file(path, std::ios::binary),
          m_builder(std::make_unique<Flatbuffers::OTAPacketBuilder>()),
          m_robot_controller(controller) {
    }
    bool perform_ota();
    void restart();

  private:
    bool start_ota();
    bool write_ota(std::vector<uint8_t> &transmit_buffer);
    bool ota_end();

    uint16_t m_sequence_num = 0;
    uint8_t m_module_id;
    std::ifstream m_file;
    std::unique_ptr<Flatbuffers::OTAPacketBuilder> m_builder;
    std::shared_ptr<RobotController> m_robot_controller;
};

#endif // REMOTEMANAGEMENT_H
