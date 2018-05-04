#include "Server.h"

std::mutex Server::mtx;

Server::Server(const char *ip, unsigned short port) {
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
}

Server::Server(const Server &source) {
    s_socket = source.s_socket;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = source.address.sin_family;
    address.sin_port = source.address.sin_port;
    address.sin_addr.s_addr = source.address.sin_addr.s_addr;
}

Server::~Server() {
    close(s_socket);
}

