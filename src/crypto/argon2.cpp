#include "crypto/argon2.hpp"
#include <cstring>

void Argon2::hash(const uint8_t* in, size_t inlen, const uint8_t* salt, size_t saltlen, uint8_t* out, size_t outlen) {
    // Placeholder: Implement Argon2 (specifically Argon2id for Monero)
    memset(out, 0, outlen);
}

std::vector<uint8_t> Argon2::hash(const std::vector<uint8_t>& data, const std::vector<uint8_t>& salt, size_t outlen) {
    std::vector<uint8_t> out(outlen);
    hash(data.data(), data.size(), salt.data(), salt.size(), out.data(), outlen);
    return out;
}
