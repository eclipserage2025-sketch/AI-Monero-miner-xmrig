#include <iostream>
#include <cassert>
#include "ai_optimizer.hpp"

int main() {
    AIOptimizer optimizer;
    MinerMetrics highLoad = {100.0, 90.0, 9.0, 400.0};
    MinerMetrics lowLoad = {1000.0, 40.0, 1.0, 20.0};

    int currentThreads = 8;
    int optimizedHigh = optimizer.optimize(highLoad, currentThreads);
    int optimizedLow = optimizer.optimize(lowLoad, currentThreads);

    std::cout << "High load optimized threads: " << optimizedHigh << std::endl;
    std::cout << "Low load optimized threads: " << optimizedLow << std::endl;

    assert(optimizedHigh <= currentThreads);
    assert(optimizedLow >= currentThreads);

    std::cout << "AI Optimizer test passed!" << std::endl;
    return 0;
}
