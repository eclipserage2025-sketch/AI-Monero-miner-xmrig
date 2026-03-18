#include "worker.hpp"
#include "randomx/randomx_vm.hpp"
#include <iostream>
#include <chrono>
#include <vector>
#include <cstring>

CpuWorker::CpuWorker(int id) : m_id(id), m_running(false), m_hashes(0), m_newJob(false) {}

CpuWorker::~CpuWorker() {
    stop();
}

void CpuWorker::start() {
    m_running = true;
    m_thread = std::thread(&CpuWorker::run, this);
}

void CpuWorker::stop() {
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void CpuWorker::setJob(const Job& job) {
    m_currentJob = job;
    m_newJob = true;
}

uint64_t CpuWorker::getHashes() const {
    return m_hashes;
}

void CpuWorker::run() {
    RandomXVM vm;
    uint32_t nonce = (uint32_t)m_id * 0x10000000; // Offset nonce per worker
    uint8_t output[32];

    while (m_running) {
        if (!m_currentJob.job_id.empty()) {
            // Actual mining loop:
            // 1. Update job if new
            if (m_newJob) {
                // Parse job blob into input...
                m_newJob = false;
            }

            // 2. Add nonce to input blob (at byte offset 39 for Monero)
            std::vector<uint8_t> input(80, 0); // Simulated blob
            memcpy(input.data(), m_currentJob.blob.data(), std::min(m_currentJob.blob.size(), (size_t)80));
            memcpy(input.data() + 39, &nonce, sizeof(nonce));

            // 3. Execute RandomX VM
            vm.execute(input, output);

            // 4. Check result against target
            // uint64_t* result = (uint64_t*)(output + 24);
            // uint64_t target = std::stoull(m_currentJob.target, nullptr, 16);
            // if (*result < target) { submit(); }

            m_hashes++;
            nonce++;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
