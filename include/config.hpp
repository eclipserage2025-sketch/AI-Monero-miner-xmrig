#pragma once

#include <string>
#include <map>

class Config {
public:
    Config();
    ~Config();

    bool load(const std::string& filename);

    // Pool settings
    std::string pool_url;
    int pool_port;
    std::string pool_user;
    std::string pool_pass;

    // AI settings
    bool ai_enabled;
    int ai_optimization_interval;
    int ai_max_threads;
    int ai_min_threads;

    // CPU settings
    bool cpu_enabled;
    int cpu_threads;

    // GPU settings
    bool gpu_enabled;
    std::string gpu_platform;

private:
    std::string findValue(const std::string& content, const std::string& key);
};
