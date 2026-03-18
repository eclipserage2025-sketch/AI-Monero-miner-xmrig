#include "crypto/blake2b.hpp"
#include <cstring>

static const uint64_t blake2b_iv[8] = {
    0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
    0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
    0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

static const uint8_t blake2b_sigma[12][16] = {
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
    { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 },
    { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 },
    {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 },
    {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 },
    {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 },
    { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 },
    { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 },
    {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 },
    { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0 },
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 },
    { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 }
};

static inline uint64_t rotr64(uint64_t x, int n) {
    return (x >> n) | (x << (64 - n));
}

#define G(a,b,c,d,m0,m1) \
    do { \
        a = a + b + m0; \
        d = rotr64(d ^ a, 32); \
        c = c + d; \
        b = rotr64(b ^ c, 24); \
        a = a + b + m1; \
        d = rotr64(d ^ a, 16); \
        c = c + d; \
        b = rotr64(b ^ c, 63); \
    } while (0)

#define ROUND(v,m,s) \
    do { \
        G(v[0], v[4], v[8],  v[12], m[s[0]], m[s[1]]); \
        G(v[1], v[5], v[9],  v[13], m[s[2]], m[s[3]]); \
        G(v[2], v[6], v[10], v[14], m[s[4]], m[s[5]]); \
        G(v[3], v[7], v[11], v[15], m[s[6]], m[s[7]]); \
        G(v[0], v[5], v[10], v[15], m[s[8]], m[s[9]]); \
        G(v[1], v[6], v[11], v[12], m[s[10]], m[s[11]]); \
        G(v[2], v[7], v[8],  v[13], m[s[12]], m[s[13]]); \
        G(v[3], v[4], v[9],  v[14], m[s[14]], m[s[15]]); \
    } while (0)

void Blake2b::hash(const uint8_t* in, size_t inlen, uint8_t* out, size_t outlen) {
    uint64_t h[8];
    uint64_t v[16];
    uint64_t m[16];

    memcpy(h, blake2b_iv, 64);
    h[0] ^= 0x01010000 ^ outlen;

    // Simplified for a single block for now
    memset(m, 0, 128);
    memcpy(m, in, inlen < 128 ? inlen : 128);

    memcpy(v, h, 64);
    memcpy(v + 8, blake2b_iv, 64);
    v[12] ^= inlen;
    v[14] ^= 0xFFFFFFFFFFFFFFFFULL; // Final block

    for (int i = 0; i < 12; ++i) {
        ROUND(v, m, blake2b_sigma[i]);
    }

    for (int i = 0; i < 8; ++i) {
        h[i] ^= v[i] ^ v[i + 8];
    }

    memcpy(out, h, outlen);
}

std::vector<uint8_t> Blake2b::hash(const std::vector<uint8_t>& data, size_t outlen) {
    std::vector<uint8_t> out(outlen);
    hash(data.data(), data.size(), out.data(), outlen);
    return out;
}
