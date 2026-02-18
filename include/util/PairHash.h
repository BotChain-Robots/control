
#ifndef CONTROL_PAIRHASH_H
#define CONTROL_PAIRHASH_H

#include <unordered_map>

// Custom hash function for std::pair
template <typename T> struct pair_hash {
    std::size_t operator()(const std::pair<T, T> &p) const {
        return std::hash<T>()(p.first) ^ (std::hash<T>()(p.second) << 1);
    }
};

#endif // CONTROL_PAIRHASH_H
