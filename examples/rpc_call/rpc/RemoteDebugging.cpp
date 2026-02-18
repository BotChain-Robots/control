//
// Created by Johnathon Slightham on 2026-02-16.
//

#include "rpc/RemoteDebugging.h"

std::string RemoteDebugging::get_task_manager(uint8_t module_id) {
    const auto maybe = m_robot_controller->remote_call(2, module_id, {});
    if (maybe) {
        std::string str((*maybe)->begin(), (*maybe)->end());
        return str;
    }
    return "";
}

std::string RemoteDebugging::get_logs(uint8_t module_id) {
    const auto maybe = m_robot_controller->remote_call(3, module_id, {});
    if (maybe) {
        std::string str((*maybe)->begin(), (*maybe)->end());
        return str;
    }
    return "";
}
