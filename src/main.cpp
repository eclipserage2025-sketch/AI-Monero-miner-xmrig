#include <iostream>
#include <thread>
#include <chrono>
#include "network.hpp"
#include "miner.hpp"

int main() {
    std::cout << "AI-Monero-Miner-XMRig starting up..." << std::endl;

    if (!Network::init()) {
        std::cerr << "Failed to initialize network" << std::endl;
        return 1;
    }

    std::cout << "Network initialized." << std::endl;

    // Use placeholder wallet for demonstration
    std::string wallet = "486vXn1T68pM7D6D8v...example_wallet_address...";
    Miner miner("pool.supportxmr.com", 3333, wallet);

    // Add 4 workers (placeholder count)
    miner.addCpuWorker(4);

    // Start mining
    miner.start();

    // Loop to show status updates every 10 seconds
    for (int i = 0; i < 6; ++i) { // 1 minute total
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout << "Total hashes: " << miner.getTotalHashes() << std::endl;
    }

    miner.stop();
    Network::cleanup();
    std::cout << "Miner stopped." << std::endl;
    return 0;
}
