#include "stratum.hpp"
#include <iostream>
#include <sstream>

StratumClient::StratumClient(const std::string& host, int port, const std::string& wallet)
    : m_host(host), m_port(port), m_wallet(wallet) {}

StratumClient::~StratumClient() {
    m_network.disconnect();
}

bool StratumClient::login() {
    if (!m_network.connect(m_host, m_port)) {
        return false;
    }

    std::stringstream ss;
    ss << "{\"id\":1,\"jsonrpc\":\"2.0\",\"method\":\"login\",\"params\":{"
       << "\"login\":\"" << m_wallet << "\","
       << "\"pass\":\"x\","
       << "\"agent\":\"AI-Miner/1.0\""
       << "}}\n";

    if (!m_network.send(ss.str())) {
        return false;
    }

    std::string response = m_network.receive();
    parseResponse(response);
    return true;
}

bool StratumClient::submit(const std::string& job_id, uint32_t nonce, const std::string& result) {
    std::stringstream ss;
    ss << "{\"id\":1,\"jsonrpc\":\"2.0\",\"method\":\"submit\",\"params\":{"
       << "\"id\":\"" << job_id << "\","
       << "\"job_id\":\"" << job_id << "\","
       << "\"nonce\":\"" << std::hex << nonce << std::dec << "\","
       << "\"result\":\"" << result << "\""
       << "}}\n";

    return m_network.send(ss.str());
}

void StratumClient::setJobHandler(std::function<void(const Job&)> handler) {
    m_jobHandler = handler;
}

void StratumClient::listen() {
    while (true) {
        std::string response = m_network.receive();
        if (response.empty()) break;
        parseResponse(response);
    }
}

void StratumClient::parseResponse(const std::string& response) {
    // Basic manual parsing of job notification for RandomX
    // Example: {"jsonrpc":"2.0","method":"job","params":{"blob":"...","job_id":"...","target":"...","height":...}}
    if (response.find("\"method\":\"job\"") != std::string::npos) {
        Job job;
        auto findField = [&](const std::string& field) -> std::string {
            size_t pos = response.find("\"" + field + "\":\"");
            if (pos == std::string::npos) return "";
            pos += field.length() + 4;
            size_t end = response.find("\"", pos);
            return response.substr(pos, end - pos);
        };

        job.job_id = findField("job_id");
        job.blob = findField("blob");
        job.target = findField("target");

        // Height is usually an int
        size_t hpos = response.find("\"height\":");
        if (hpos != std::string::npos) {
            job.height = std::stoul(response.substr(hpos + 9));
        }

        if (m_jobHandler) {
            m_jobHandler(job);
        }
    }
}
