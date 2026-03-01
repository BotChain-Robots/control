//
// Created by Johnathon Slightham on 2025-11-13.
//

#include <chrono>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <system_error>
#include <thread>

#include "flatbuffers_generated/SensorMessage_generated.h"
#include "spdlog/spdlog.h"

#include "ModuleFactory.h"
#include "flatbuffers/RobotConfigurationBuilder.h"
#include "flatbuffers/SensorMessageBuilder.h"
#include "flatbuffers/TopologyMessageBuilder.h"
#include "flatbuffers_generated/TopologyMessage_generated.h"
#include "libcontrol.h"
#include "util/Map.h"

#define SCAN_DURATION_MS 2000
#define ACTUATOR_CMD_TAG 5
#define TOPOLOGY_CMD_TAG 6
#define SENSOR_CMD_TAG 8
#define NUM_CHANNELS 4
#define TOPOLOGY_BUFFER_SIZE 1024
#define SENSOR_BUFFER_SIZE 1024
#define NETWORK_CONFIG_FETCH_RATE 3s
#define MODULE_EXPIRE_TIME 3s
#define CONTROL_MESSAGE_FREQUENCY 50ms

using namespace std::chrono_literals;

RobotController::~RobotController() {
    m_stop_thread = true;
    if (m_metadata_loop.joinable())
        m_metadata_loop.join();
    if (m_transmit_loop.joinable())
        m_transmit_loop.join();
    if (m_configuration_loop.joinable())
        m_configuration_loop.join();
    if (m_sensor_loop.joinable())
        m_sensor_loop.join();
    if (m_expiry_looop.joinable())
        m_expiry_looop.join();
}

std::vector<std::weak_ptr<Module>> RobotController::getModules() {
    std::vector<std::weak_ptr<Module>> out;
    std::shared_lock lock(m_module_lock);
    out.reserve(m_id_to_module.size());
    for (const auto m : map_to_values(m_id_to_module)) {
        out.emplace_back(m);
    }
    return out;
}

std::vector<Flatbuffers::ModuleConnectionInstance> RobotController::getConnections() {
    std::vector<Flatbuffers::ModuleConnectionInstance> out;
    std::shared_lock lock(m_connection_lock);

    for (auto const &[_, value] : m_connection_map) {
        for (const auto conn : value) {
            out.push_back(conn);
        }
    }
    return out;
}

std::vector<Flatbuffers::ModuleInstance> RobotController::getModuleList() {
    std::vector<Flatbuffers::ModuleInstance> out;
    std::shared_lock lock(m_module_lock);
    for (auto const &[key, value] : m_id_to_module) {
        out.push_back({key, value->get_type()});
    }
    return out;
}

std::optional<std::weak_ptr<Module>> RobotController::getModule(uint8_t device_id) {
    std::shared_lock lock(m_module_lock);
    if (m_id_to_module.contains(device_id)) {
        return m_id_to_module[device_id];
    } else {
        return std::nullopt;
    }
}

void RobotController::resetModules() {
    std::unique_lock module_lock(m_module_lock);
    std::unique_lock conn_lock(m_connection_lock);
    m_id_to_module.clear();
    m_connection_map.clear();
}

void RobotController::fetchDirectlyConnectedModules(bool block) {
    spdlog::info("[Control] Fetching modules from network");
    // Capture m_messaging_interface by value (shared_ptr) so the detached
    // thread does not hold a dangling reference to 'this'.
    auto messaging = m_messaging_interface;
    auto t = std::thread([messaging] {
        auto out = messaging->find_connected_modules(std::chrono::milliseconds(SCAN_DURATION_MS));
        spdlog::info("[Control] Found {} modules on the network", out.size());
    });

    if (block) {
        t.join();
    } else {
        t.detach();
    }
}

void RobotController::metadata_loop() {
    unsigned char buf[TOPOLOGY_BUFFER_SIZE];
    const auto builder = std::make_unique<Flatbuffers::TopologyMessageBuilder>();
    while (!m_stop_thread) {
        if (auto result =
                m_messaging_interface->recv(buf, TOPOLOGY_BUFFER_SIZE, TOPOLOGY_CMD_TAG)) {
            const auto &[rx_size, from] = *result;

            flatbuffers::Verifier verifier(buf, rx_size);
            if (!Messaging::VerifyTopologyMessageBuffer(verifier)) {
                continue;
            }

            const auto metadata = builder->parse_topology_message(reinterpret_cast<uint8_t *>(buf));
            std::unique_lock lock(m_module_lock);
            if (!m_id_to_module.contains(metadata->module_id())) {
                spdlog::info("[Control] Creating module entry for {}", metadata->module_id());
                m_id_to_module.insert(
                    {metadata->module_id(),
                     ModuleFactory::createModule(metadata->module_id(), metadata->module_type(),
                                                 m_messaging_interface)});
            }

            m_id_to_module[metadata->module_id()]->update_module_metadata(*metadata);

            std::unique_lock conn_lock(m_connection_lock);
            std::vector<Flatbuffers::ModuleConnectionInstance> conns;
            for (uint8_t i = 1; i < NUM_CHANNELS; i++) {
                if (metadata->channel_to_module()->Get(i) > 0) {
                    conns.push_back(Flatbuffers::ModuleConnectionInstance{
                        .from_module_id = metadata->module_id(),
                        .to_module_id = metadata->channel_to_module()->Get(i),
                        .from_socket = i,
                        .to_socket = 0,
                        .orientation =
                            static_cast<Orientation>(metadata->channel_to_orientation()->Get(i))});
                }
            }

            m_connection_map[metadata->module_id()] = conns;
        }
    }
}

void RobotController::transmit_loop() {
    const auto builder = std::make_unique<Flatbuffers::TopologyMessageBuilder>();
    while (!m_stop_thread) {
        std::this_thread::sleep_for(CONTROL_MESSAGE_FREQUENCY);
        std::shared_lock lock(m_module_lock);
        for (const auto [id, module] : m_id_to_module) {
            auto out = module->get_actuation_message();

            if (out.size() > 0) {
                m_messaging_interface->send(out.data(), out.size(), id, ACTUATOR_CMD_TAG, false);
            }
        }
    }
}

void RobotController::configuration_loop() {
    while (!m_stop_thread) {
        std::this_thread::sleep_for(NETWORK_CONFIG_FETCH_RATE);
        fetchDirectlyConnectedModules(true);
    }
}

void RobotController::expiry_loop() {
    while (!m_stop_thread) {
        std::this_thread::sleep_for(MODULE_EXPIRE_TIME);

        std::unordered_set<uint8_t> delete_modules{};

        std::unique_lock module_lock(m_module_lock);
        std::unique_lock connection_lock(m_connection_lock);

        for (auto it = m_id_to_module.begin(); it != m_id_to_module.end();) {
            if (it->second->get_last_updated_time() <
                std::chrono::system_clock::now() - MODULE_EXPIRE_TIME) {
                delete_modules.emplace(it->first);
                it = m_id_to_module.erase(it);
            } else {
                ++it;
            }
        }

        // Remove connections involving expired modules.
        for (auto it = m_connection_map.begin(); it != m_connection_map.end();) {
            if (delete_modules.contains(it->first)) {
                it = m_connection_map.erase(it);
            } else {
                // Remove individual connections pointing to an expired module.
                auto &conns = it->second;
                conns.erase(std::remove_if(conns.begin(), conns.end(),
                                           [&](const Flatbuffers::ModuleConnectionInstance &c) {
                                               return delete_modules.contains(c.to_module_id);
                                           }),
                            conns.end());
                ++it;
            }
        }
    }
}

void RobotController::sensor_loop() {
    unsigned char buf[SENSOR_BUFFER_SIZE];
    const auto builder = std::make_unique<Flatbuffers::SensorMessageBuilder>();
    while (!m_stop_thread) {
        if (auto result = m_messaging_interface->recv(buf, SENSOR_BUFFER_SIZE, SENSOR_CMD_TAG)) {
            const auto &[rx_size, from] = *result;

            flatbuffers::Verifier verifier(buf, rx_size);
            if (!Messaging::VerifySensorMessageBuffer(verifier)) {
                continue;
            }

            const auto sensor_message =
                builder->parse_sensor_message(reinterpret_cast<uint8_t *>(buf));
            if (sensor_message->values()->size() != sensor_message->values_type()->size()) {
                spdlog::error("[Control] Got a sensor message with different value "
                              "({}) and type ({}) sizes",
                              sensor_message->values()->size(),
                              sensor_message->values_type()->size());
                continue;
            }

            std::shared_lock module_lock(m_module_lock);
            if (!m_id_to_module.contains(from)) {
                continue;
            }

            for (int i = 0; i < sensor_message->values()->size(); i++) {
                if (auto maybe_value = Flatbuffers::SensorMessageBuilder::build_sensor_value(
                        static_cast<Messaging::SensorValue>(sensor_message->values_type()->Get(i)),
                        sensor_message->values()->Get(i))) {
                    m_id_to_module[from]->update_sensor_data(*maybe_value);
                }
            }
        }
    }
}

std::optional<std::unique_ptr<std::vector<uint8_t>>>
RobotController::remote_call(uint8_t function_tag, uint8_t module,
                             const std::vector<uint8_t> &parameters) {
    return m_messaging_interface->remote_call(function_tag, module, parameters);
}
