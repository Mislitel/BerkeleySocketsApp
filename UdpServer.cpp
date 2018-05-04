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
        perror(SUBJECT_UDP_SERVER);
        throw std::runtime_error(MESSAGE_SOCKET_FAIL);
    }
    // Socket bind
    if (bind(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        perror(SUBJECT_UDP_SERVER);
        throw std::runtime_error(MESSAGE_BIND_FAIL);
    }
    std::cout << "Server UDP on " << inet_ntoa(address.sin_addr) <<
              ":" << htons(address.sin_port) << " is listening..." << std::endl;
    mtx.unlock();
    udp_processing();
}

void UdpServer::udp_processing() {
    while (true) {
        ssize_t r_len = receiveMessage(s_socket, buffer, MAX_MESSAGE_SIZE);
        if (r_len == 0) {
            std::cerr << "UDP: No message from " << inet_ntoa(remote.sin_addr)
                      << ":" << htons(remote.sin_port) << std::endl;
        }
        if (r_len < 0) {
            perror(SUBJECT_UDP_SERVER);
            throw std::runtime_error(MESSAGE_RECEIVE_FAIL);
        }
        mtx.lock();
        std::cout << "UDP:" << inet_ntoa(remote.sin_addr)
                  << ":" << htons(remote.sin_port) << ": " << buffer << std::endl;
        Parser::parse(buffer);
        mtx.unlock();
        ssize_t s_len = sendMessage(s_socket, buffer, static_cast<uint32_t>(strlen(buffer) + 1));
        if (s_len == 0) {
            std::cerr << "UDP: Cannot send to " << inet_ntoa(remote.sin_addr)
                      << ":" << htons(remote.sin_port) << std::endl;
        }
        if (s_len <= 0) {
            perror(SUBJECT_UDP_SERVER);
            throw std::runtime_error(MESSAGE_SEND_FAIL);
        }
    }
}