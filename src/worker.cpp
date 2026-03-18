#include "worker.hpp"
#include <iostream>
#include <chrono>

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
    while (m_running) {
        if (!m_currentJob.job_id.empty()) {
            // Simulated mining loop until RandomX is implemented
            // In reality, this will call RandomX virtual machine code
            m_hashes++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Slow down for simulation
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
