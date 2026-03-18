#pragma once

#include "network.hpp"
#include <string>
#include <functional>

struct Job {
    std::string job_id;
    std::string blob;
    std::string target;
    uint32_t height;
};

class StratumClient {
public:
    StratumClient(const std::string& host, int port, const std::string& wallet);
    ~StratumClient();

    bool login();
    bool submit(const std::string& job_id, uint32_t nonce, const std::string& result);
    void setJobHandler(std::function<void(const Job&)> handler);
    void listen();

private:
    std::string m_host;
    int m_port;
    std::string m_wallet;
    Network m_network;
    std::function<void(const Job&)> m_jobHandler;

    void parseResponse(const std::string& response);
};
