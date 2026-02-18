
#include <chrono>
#include <ranges>

#include "Module.h"

std::vector<neighbour> Module::get_neighbours() {
    return m_neighbours;
}

uint8_t Module::get_device_id() {
    return m_device_id;
}

ModuleType Module::get_type() {
    return m_module_type;
}

Messaging::ConnectionType Module::get_connection_type() {
    return m_connection_type;
}

uint8_t Module::get_leader() {
    return m_leader;
}

std::chrono::time_point<std::chrono::system_clock> Module::get_last_updated_time() {
    return m_last_updated;
}

void Module::update_module_metadata(const Messaging::TopologyMessage &message) {
    m_module_type = message.module_type();
    m_connection_type = message.connection();
    m_leader = message.leader();

    m_last_updated = std::chrono::system_clock::now();

    m_neighbours.clear();
    for (auto [id, ori] :
         std::views::zip(*message.channel_to_module(), *message.channel_to_orientation())) {
        m_neighbours.emplace_back(neighbour{id, static_cast<Orientation>(ori)});
    }
}
