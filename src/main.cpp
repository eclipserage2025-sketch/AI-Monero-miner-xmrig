#include <iostream>
#include <thread>
#include <chrono>
#include "network.hpp"
#include "miner.hpp"
#include "config.hpp"

int main() {
    std::cout << "AI-Monero-Miner-XMRig starting up..." << std::endl;

    Config config;
    if (config.load("config.json")) {
        std::cout << "Configuration loaded from config.json" << std::endl;
    } else {
        std::cout << "Using default configuration." << std::endl;
    }

    if (!Network::init()) {
        std::cerr << "Failed to initialize network" << std::endl;
        return 1;
    }

    std::cout << "Network initialized. Connecting to " << config.pool_url << ":" << config.pool_port << std::endl;

    Miner miner(config.pool_url, config.pool_port, config.pool_user);

    if (config.cpu_enabled) {
        std::cout << "Enabling CPU mining with " << config.cpu_threads << " threads." << std::endl;
        miner.addCpuWorker(config.cpu_threads);
    }

    if (config.gpu_enabled) {
        std::cout << "Enabling GPU mining on platform " << config.gpu_platform << "." << std::endl;
        miner.addGpuWorker(1, config.gpu_platform);
    }

    miner.start();

    // Main execution loop
    for (int i = 0; i < 6; ++i) { // 1 minute
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "Hashrate: " << miner.getTotalHashes() / 10.0 << " H/s (simulated)" << std::endl;

        if (config.ai_enabled) {
            miner.optimize();
        }
    }

    miner.stop();
    Network::cleanup();
    std::cout << "Miner stopped." << std::endl;
    return 0;
}
