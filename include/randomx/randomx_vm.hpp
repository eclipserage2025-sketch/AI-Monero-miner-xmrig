#pragma once

#include <cstdint>
#include <vector>

class RandomXVM {
public:
    RandomXVM();
    ~RandomXVM();

    void init(const std::vector<uint8_t>& key);
    void execute(const std::vector<uint8_t>& input, uint8_t* output);

private:
    // Core VM state (registers, memory)
    uint64_t m_r[8]; // General-purpose registers
    uint8_t* m_scratchpad; // 2MB scratchpad

    void runProgram();
};
