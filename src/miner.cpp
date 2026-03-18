#include "miner.hpp"
#include <iostream>
#include <thread>
#include <numeric>

Miner::Miner(const std::string& host, int port, const std::string& wallet)
    : m_host(host), m_port(port), m_wallet(wallet), m_running(false) {
    m_client = std::make_unique<StratumClient>(host, port, wallet);
    m_optimizer = std::make_unique<AIOptimizer>();
}

Miner::~Miner() {
    stop();
}

void Miner::start() {
    m_running = true;
    m_client->setJobHandler(std::bind(&Miner::handleJob, this, std::placeholders::_1));

    if (m_client->login()) {
        std::cout << "Connected to pool and logged in." << std::endl;

        // Start workers
        for (auto& worker : m_workers) {
            worker->start();
        }

        // Listen for jobs in a separate thread
        std::thread listenThread([this]() {
            m_client->listen();
        });
        listenThread.detach();
    } else {
        std::cerr << "Failed to login to pool." << std::endl;
    }
}

void Miner::stop() {
    m_running = false;
    for (auto& worker : m_workers) {
        worker->stop();
    }
}

void Miner::addCpuWorker(int count) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (int i = 0; i < count; ++i) {
        m_workers.push_back(std::make_unique<CpuWorker>(m_workers.size()));
    }
}

void Miner::addGpuWorker(int count, const std::string& platform) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (int i = 0; i < count; ++i) {
        m_workers.push_back(std::make_unique<GpuWorker>(m_workers.size(), platform));
    }
}

void Miner::handleJob(const Job& job) {
    std::cout << "New job received: " << job.job_id << " Height: " << job.height << std::endl;
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& worker : m_workers) {
        worker->setJob(job);
    }
}

uint64_t Miner::getTotalHashes() const {
    uint64_t total = 0;
    for (const auto& worker : m_workers) {
        total += worker->getHashes();
    }
    return total;
}

void Miner::optimize() {
    MinerMetrics metrics = getMetrics();
    int currentThreads = m_workers.size();
    int newThreads = m_optimizer->optimize(metrics, currentThreads);

    if (newThreads != currentThreads) {
        std::cout << "AI suggests changing threads from " << currentThreads << " to " << newThreads << std::endl;
    }
}

MinerMetrics Miner::getMetrics() {
    MinerMetrics m;
    m.hashrate = (double)getTotalHashes();
    m.cpu_temp = 65.0;
    m.system_load = 2.5;
    m.network_latency = 120.0;
    return m;
}
