//
// Created by Johnathon Slightham on 2026-02-16.
//

#ifndef NEW_DEV_TOOLS_REMOTEDEBUGGING_H
#define NEW_DEV_TOOLS_REMOTEDEBUGGING_H

#include <cstdint>
#include <string>

#include "libcontrol.h"

class RemoteDebugging {
  public:
    RemoteDebugging(std::shared_ptr<RobotController> controller) : m_robot_controller(controller) {
    }
    std::string get_task_manager(uint8_t module_id);
    std::string get_logs(uint8_t module_id);

  private:
    std::shared_ptr<RobotController> m_robot_controller;
};

#endif //NEW_DEV_TOOLS_REMOTEDEBUGGING_H
