#pragma once

#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <iostream>

struct MinerMetrics {
    double hashrate;
    double cpu_temp;
    double system_load;
    double network_latency;
};

class AIOptimizer {
public:
    AIOptimizer();
    ~AIOptimizer();

    // Collect metrics and decide on the next action (e.g., adjust thread count)
    int optimize(const MinerMetrics& metrics, int currentThreads);

private:
    // Simple 2-layer Neural Network from scratch for optimization
    std::vector<double> m_weights;
    std::vector<double> m_bias;

    double sigmoid(double x);
    double predict(const std::vector<double>& inputs);
    void train(const std::vector<double>& inputs, double target);
};
