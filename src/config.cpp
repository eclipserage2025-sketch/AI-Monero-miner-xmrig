#include "config.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Config::Config() : pool_port(3333), ai_enabled(true), ai_optimization_interval(60),
                   ai_max_threads(32), ai_min_threads(1), cpu_enabled(true),
                   cpu_threads(4), gpu_enabled(false), gpu_platform("OpenCL") {}

Config::~Config() {}

bool Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open config file: " << filename << ". Using defaults." << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto parseString = [&](const std::string& key) -> std::string {
        size_t pos = content.find("\"" + key + "\":");
        if (pos == std::string::npos) return "";
        pos = content.find("\"", pos + key.length() + 3);
        if (pos == std::string::npos) return "";
        size_t end = content.find("\"", pos + 1);
        return content.substr(pos + 1, end - pos - 1);
    };

    auto parseInt = [&](const std::string& key, int def) -> int {
        size_t pos = content.find("\"" + key + "\":");
        if (pos == std::string::npos) return def;
        pos += key.length() + 3;
        size_t end = content.find_first_of(",}", pos);
        return std::stoi(content.substr(pos, end - pos));
    };

    auto parseBool = [&](const std::string& key, bool def) -> bool {
        size_t pos = content.find("\"" + key + "\":");
        if (pos == std::string::npos) return def;
        pos += key.length() + 3;
        // Search for true or false after the colon
        size_t truePos = content.find("true", pos);
        size_t falsePos = content.find("false", pos);
        if (truePos != std::string::npos && (falsePos == std::string::npos || truePos < falsePos)) {
            return true;
        }
        return false;
    };

    pool_url = parseString("url");
    pool_port = parseInt("port", 3333);
    pool_user = parseString("user");
    pool_pass = parseString("pass");

    // Rough parsing for nested structures
    size_t aiPos = content.find("\"ai\":");
    if (aiPos != std::string::npos) {
        std::string aiSection = content.substr(aiPos);
        ai_enabled = parseBool("enabled", true);
        ai_optimization_interval = parseInt("optimization_interval", 60);
        ai_max_threads = parseInt("max_threads", 32);
        ai_min_threads = parseInt("min_threads", 1);
    }

    size_t cpuPos = content.find("\"cpu\":");
    if (cpuPos != std::string::npos) {
        std::string cpuSection = content.substr(cpuPos);
        // Find first "enabled" after "cpu"
        size_t enPos = cpuSection.find("\"enabled\":");
        if (enPos != std::string::npos) {
            cpu_enabled = cpuSection.substr(enPos + 10, 4) == "true";
        }
        size_t thrPos = cpuSection.find("\"threads\":");
        if (thrPos != std::string::npos) {
            cpu_threads = std::stoi(cpuSection.substr(thrPos + 10));
        }
    }

    size_t gpuPos = content.find("\"gpu\":");
    if (gpuPos != std::string::npos) {
        std::string gpuSection = content.substr(gpuPos);
        size_t enPos = gpuSection.find("\"enabled\":");
        if (enPos != std::string::npos) {
            gpu_enabled = gpuSection.substr(enPos + 10, 4) == "true";
        }
        size_t platPos = gpuSection.find("\"platform\":");
        if (platPos != std::string::npos) {
            size_t start = gpuSection.find("\"", platPos + 11);
            size_t end = gpuSection.find("\"", start + 1);
            gpu_platform = gpuSection.substr(start + 1, end - start - 1);
        }
    }

    return true;
}
