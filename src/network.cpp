#include <cstring>
#include "network.hpp"
#include <iostream>

bool Network::init() {
#ifdef _WIN32
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
    return true;
#endif
}

void Network::cleanup() {
#ifdef _WIN32
    WSACleanup();
#endif
}

Network::Network() : m_socket(INVALID_SOCKET) {}

Network::~Network() {
    disconnect();
}

bool Network::connect(const std::string& host, int port) {
    struct addrinfo hints, *res = nullptr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res) != 0) {
        return false;
    }

    m_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (m_socket == INVALID_SOCKET) {
        freeaddrinfo(res);
        return false;
    }

    if (::connect(m_socket, res->ai_addr, (int)res->ai_addrlen) == SOCKET_ERROR) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        freeaddrinfo(res);
        return false;
    }

    freeaddrinfo(res);
    return true;
}

void Network::disconnect() {
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}

bool Network::send(const std::string& data) {
    if (m_socket == INVALID_SOCKET) return false;
    int sent = ::send(m_socket, data.c_str(), (int)data.length(), 0);
    return sent != SOCKET_ERROR;
}

std::string Network::receive() {
    if (m_socket == INVALID_SOCKET) return "";
    char buffer[4096];
    int received = ::recv(m_socket, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        return std::string(buffer);
    }
    return "";
}
