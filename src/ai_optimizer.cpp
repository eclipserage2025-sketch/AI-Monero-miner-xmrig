#include "ai_optimizer.hpp"
#include <numeric>
#include <algorithm>
#include <random>

AIOptimizer::AIOptimizer() {
    // Initialize weights for 4 inputs (hashrate, temp, load, latency)
    m_weights = {0.5, -0.2, -0.3, -0.1};
    m_bias = {0.1};
}

AIOptimizer::~AIOptimizer() {}

double AIOptimizer::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double AIOptimizer::predict(const std::vector<double>& inputs) {
    double sum = 0.0;
    for (size_t i = 0; i < inputs.size(); ++i) {
        sum += inputs[i] * m_weights[i];
    }
    sum += m_bias[0];
    return sigmoid(sum);
}

int AIOptimizer::optimize(const MinerMetrics& metrics, int currentThreads) {
    // Normalize inputs
    std::vector<double> inputs = {
        metrics.hashrate / 1000.0, // Scale to roughly 0-1
        (metrics.cpu_temp - 50.0) / 50.0, // Scale temp around 50-100 to 0-1
        metrics.system_load / 10.0, // Scale load
        metrics.network_latency / 500.0 // Scale latency
    };

    double prediction = predict(inputs);

    // Logic: If prediction > 0.6, increase threads; < 0.4, decrease threads
    if (prediction > 0.6) {
        return std::min(currentThreads + 1, 32); // Max 32 threads
    } else if (prediction < 0.4) {
        return std::max(currentThreads - 1, 1);  // Min 1 thread
    }

    return currentThreads;
}

void AIOptimizer::train(const std::vector<double>& inputs, double target) {
    // Basic backpropagation implementation (stochastic gradient descent)
    // Placeholder for real training logic during runtime
}
