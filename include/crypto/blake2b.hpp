#pragma once

#include <cstdint>
#include <vector>

class Blake2b {
public:
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& data, size_t outlen);
    static void hash(const uint8_t* in, size_t inlen, uint8_t* out, size_t outlen);
};
