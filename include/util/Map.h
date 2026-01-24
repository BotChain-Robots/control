
#ifndef CONTROL_MAP_H
#define CONTROL_MAP_H

#include <unordered_map>
#include <vector>

template <typename K, typename V, typename H>
std::vector<V> map_to_values(const std::unordered_map<K, V, H> &map) {
    std::vector<V> out;
    out.reserve(map.size());
    for (auto const &[key, value] : map) {
        out.push_back(value);
    }
    return out;
}

#endif // CONTROL_MAP_H
