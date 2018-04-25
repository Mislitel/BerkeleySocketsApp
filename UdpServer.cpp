#include "UdpServer.h"

UdpServer::UdpServer(const char *ip, unsigned short port) : Server(ip, port) {
}

void UdpServer::start() {
    mtx.lock();
    // Socket start
    s_socket = socket(
            AF_INET,
            SOCK_DGRAM,
            IPPROTO_UDP
    );
    if (s_socket < 0) {
        perror("UDP");
        throw std::runtime_error(MESSAGE_SOCKET_FAIL);
    }
    // Socket bind
    if (bind(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        perror(MESSAGE_BIND_FAIL);
        throw std::runtime_error(MESSAGE_BIND_FAIL);
    }
    std::cout << "Server UDP on " << inet_ntoa(address.sin_addr) << ":" << htons(address.sin_port) << " is listening..." << std::endl;
    mtx.unlock();
    udp_processing();
}

void UdpServer::udp_processing() {
    sockaddr_in remote;
    socklen_t addrlen = sizeof(remote);
    while (true) {
        auto r_len = recvfrom(s_socket, buffer, MAX_MESSAGE_SIZE, 0, (sockaddr*)&remote, &addrlen);
        if (r_len <= 0) {
            perror(MESSAGE_RECEIVE_FAIL);
            throw std::runtime_error(MESSAGE_RECEIVE_FAIL);
        }
        mtx.lock();
        std::cout << "UDP:" << inet_ntoa(remote.sin_addr) << ":" << remote.sin_port << ": " << buffer << std::endl;
        Parser::parse(buffer);
        mtx.unlock();
        auto s_len = sendto(s_socket, buffer, strlen(buffer) + 1, 0, (sockaddr*)&remote, addrlen);
        if (s_len <= 0) {
            perror(MESSAGE_SEND_FAIL);
            throw std::runtime_error(MESSAGE_SEND_FAIL);
        }
    }
}