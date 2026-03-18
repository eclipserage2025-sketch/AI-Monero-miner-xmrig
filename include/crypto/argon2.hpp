#pragma once

#include <cstdint>
#include <vector>

class Argon2 {
public:
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& data, const std::vector<uint8_t>& salt, size_t outlen);
    static void hash(const uint8_t* in, size_t inlen, const uint8_t* salt, size_t saltlen, uint8_t* out, size_t outlen);
};
