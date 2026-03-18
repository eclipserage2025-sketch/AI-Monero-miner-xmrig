#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#include <string>
#include <vector>

class Network {
public:
    static bool init();
    static void cleanup();

    Network();
    ~Network();

    bool connect(const std::string& host, int port);
    void disconnect();
    bool send(const std::string& data);
    std::string receive();

private:
    SOCKET m_socket;
};
