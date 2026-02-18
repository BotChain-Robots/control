//
// Created by Johnathon Slightham on 2026-02-16.
//

#include <fstream>
#include <iostream>

#include "rpc/RemoteManagement.h"

#define MAX_PACKET_TX_ATTEMPTS 5
#define OTA_CHUNK_SIZE 1024

bool RemoteManagement::perform_ota() {
    if (!m_file) {
        return false;
    }

    if (!start_ota()) {
        // std::cout << "Fail to start OTA update" << std::endl;
        return false;
    }

    m_file.seekg(0, std::ios::end);
    std::streamsize total_size = m_file.tellg();
    m_file.seekg(0, std::ios::beg);
    // std::cout << "Total number of chunks: " << total_size/OTA_CHUNK_SIZE << std::endl;

    while (m_file) {
        // std::cout << "Top of transmit " << m_sequence_num << std::endl;

        std::vector<uint8_t> buffer(OTA_CHUNK_SIZE);
        m_file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
        std::streamsize bytes_read = m_file.gcount();

        if (bytes_read <= 0) {
            break;
        }

        if (m_sequence_num == 1 && buffer[0] != 0xE9) {
            // std::cout << "First byte of firmware must be 0xE9" << std::endl;
            return false;
        }

        // buffer.resize(bytes_read);
        if (!write_ota(buffer)) {
            // std::cout << "Failed to write" << std::endl;
            return false;
        }
    }

    ota_end();
    restart();

    return true;
}

void RemoteManagement::restart() {
    // Will never return since the module will shutdown
    m_robot_controller->remote_call(7, m_module_id, {});
}

bool RemoteManagement::start_ota() {
    // std::cout << "Starting OTA" << std::endl;
    const auto maybe = m_robot_controller->remote_call(4, m_module_id, {});
    if (maybe) {
        // std::cout << "Got valid response" << std::endl;
        m_sequence_num = 1;
        return (*maybe)->at(0) > 0;
    }
    return false;
}

bool RemoteManagement::write_ota(std::vector<uint8_t> &transmit_buffer) {
    // std::cout << "Write OTA " << std::endl;
    const auto [ota_packet, packet_size] =
        m_builder->build_ota_packet(m_sequence_num, transmit_buffer);
    std::vector<uint8_t> vec((uint8_t *)ota_packet, (uint8_t *)ota_packet + packet_size);
    int attempts = 0;
    while (attempts < MAX_PACKET_TX_ATTEMPTS) {
        const auto maybe = m_robot_controller->remote_call(5, m_module_id, vec);
        if (maybe && (*maybe)->at(0) > 0) {
            // std::cout << "Success writing OTA" << std::endl;
            m_sequence_num++;
            return true;
        }
        attempts++;
    }
    return false;
}

bool RemoteManagement::ota_end() {
    const auto maybe = m_robot_controller->remote_call(6, m_module_id, {});
    if (maybe) {
        m_sequence_num = 0;
        return (*maybe)->at(0) > 0;
    }
    return false;
}
