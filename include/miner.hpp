#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include "worker.hpp"
#include "gpu/gpu_worker.hpp"
#include "stratum.hpp"
#include "ai_optimizer.hpp"

class Miner {
public:
    Miner(const std::string& host, int port, const std::string& wallet);
    ~Miner();

    void start();
    void stop();
    void addCpuWorker(int count);
    void addGpuWorker(int count, const std::string& platform);
    void setJob(const Job& job);
    uint64_t getTotalHashes() const;
    void optimize();

private:
    std::string m_host;
    int m_port;
    std::string m_wallet;
    std::vector<std::unique_ptr<Worker>> m_workers;
    std::unique_ptr<StratumClient> m_client;
    std::unique_ptr<AIOptimizer> m_optimizer;
    std::mutex m_mutex;
    std::atomic<bool> m_running;

    void handleJob(const Job& job);
    MinerMetrics getMetrics();
};
