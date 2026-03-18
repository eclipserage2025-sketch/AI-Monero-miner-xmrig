#pragma once

#include <string>
#include <atomic>
#include <thread>
#include "stratum.hpp"

enum class WorkerType {
    CPU,
    CUDA,
    OpenCL
};

class Worker {
public:
    virtual ~Worker() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void setJob(const Job& job) = 0;
    virtual uint64_t getHashes() const = 0;
    virtual WorkerType getType() const = 0;
};

class CpuWorker : public Worker {
public:
    CpuWorker(int id);
    ~CpuWorker();

    void start() override;
    void stop() override;
    void setJob(const Job& job) override;
    uint64_t getHashes() const override;
    WorkerType getType() const override { return WorkerType::CPU; }

private:
    int m_id;
    std::atomic<bool> m_running;
    std::thread m_thread;
    std::atomic<uint64_t> m_hashes;
    Job m_currentJob;
    std::atomic<bool> m_newJob;

    void run();
};
