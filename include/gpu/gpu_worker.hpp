#pragma once

#include "worker.hpp"
#include <string>

class GpuWorker : public Worker {
public:
    GpuWorker(int id, const std::string& platform);
    ~GpuWorker();

    void start() override;
    void stop() override;
    void setJob(const Job& job) override;
    uint64_t getHashes() const override;
    WorkerType getType() const override { return WorkerType::OpenCL; }

private:
    int m_id;
    std::string m_platform;
    std::atomic<bool> m_running;
    std::thread m_thread;
    std::atomic<uint64_t> m_hashes;
    Job m_currentJob;

    void run();
};
