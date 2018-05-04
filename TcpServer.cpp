#include "TcpServer.h"

TcpServer::TcpServer(const char *ip, unsigned short port) : Server(ip, port) {
}

// Start server for listening
void TcpServer::start() {
    mtx.lock();
    // Socket start
    s_socket = socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
    );
    if (s_socket < 0) {
        perror(SUBJECT_TCP_SERVER);
        throw std::runtime_error(MESSAGE_SOCKET_FAIL);
    }
    // Socket bind
    if (bind(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        perror(SUBJECT_TCP_SERVER);
        throw std::runtime_error(MESSAGE_BIND_FAIL);
    }
    if (listen(s_socket, 1) < 0) {
        perror(SUBJECT_TCP_SERVER);
        throw std::runtime_error(MESSAGE_LISTEN_FAIL);
    }
    std::cout << "Server TCP on " << inet_ntoa(address.sin_addr)
              << ":" << htons(address.sin_port) << " is listening..." << std::endl;
    mtx.unlock();
    tcp_processing();
}

void TcpServer::tcp_processing() {
    std::vector<std::thread> clients;
    sockaddr_in remote;
    socklen_t addrlen = sizeof(remote);
    while (true) {
        int c_socket = accept(s_socket, (sockaddr*)&remote, &addrlen);
        if (c_socket < 0) {
            perror(SUBJECT_TCP_SERVER);
            throw std::runtime_error(MESSAGE_ACCEPT_FAIL);
        }
        std::cout << "Client " << inet_ntoa(remote.sin_addr)
                  << ":" << remote.sin_port << " connected" << std::endl;

        std::thread(&TcpServer::accepted, this, c_socket, remote).detach();
    }
}


void TcpServer::accepted(int c_socket, sockaddr_in remote) {
    while (true) {
        // Data receiving
        ssize_t r_len = receiveMessage(c_socket, buffer, MAX_MESSAGE_SIZE);
        if (r_len == 0) {
            std::cerr << "Client closed connection." << std::endl;
            break;
        }
        if (r_len < 0) {
            perror(MESSAGE_RECEIVE_FAIL);
            close(c_socket);
            break;
        }

        mtx.lock();
        std::cout << "TCP:" <<  inet_ntoa(remote.sin_addr)
                  << ":" << htons(remote.sin_port) << ": " << buffer << std::endl;
        // If quit
        if (strcmp(buffer, COMMAND_QUIT) == 0) {
            mtx.unlock();
            close(c_socket);
            break;
        }
        Parser::parse(buffer);
        mtx.unlock();
        // Answer sending
        ssize_t s_len = sendMessage(c_socket, buffer, static_cast<uint32_t>(strlen(buffer) + 1));
        if (s_len == 0) {
            std::cerr << "Client closed connection." << std::endl;
            break;
        }
        if (s_len < 0) {
            perror(MESSAGE_SEND_FAIL);
            close(c_socket);
            break;
        }
    }
    std::cout << "Client " << inet_ntoa(remote.sin_addr)
                           << ":" << remote.sin_port << " disconnected" << std::endl;
}