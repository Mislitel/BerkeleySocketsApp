#include "general.h"

char buffer[MAX_MESSAGE_SIZE];

// When some client is accepted (TCP only)
int accepted(int c_socket) {
    while (true) {
        // Data receiving
        auto r_len = recv(c_socket, buffer, sizeof(buffer), 0);
        if (r_len <= 0) {
            close(c_socket);
            perror(MESSAGE_RECEIVE_FAIL);
            return CODE_RECEIVE_FAIL;
        }
        std::cout << "c: " << buffer << std::endl;
        // If quit
        if (strcmp(buffer, COMMAND_QUIT) == 0) {
            close(c_socket);
            return CODE_SUCCESS;
        }
        parse(buffer);
        // Answer sending
        auto s_len = send(c_socket, buffer, strlen(buffer) + 1, 0);
        if (s_len <= 0) {
            close(c_socket);
            perror(MESSAGE_SEND_FAIL);
            return CODE_SEND_FAIL;
        }
    }
}

int tcp_processing(int s_socket) {
    sockaddr_in remote;
    socklen_t addrlen = sizeof(remote);
    while (true) {
        int c_socket = accept(s_socket, (sockaddr*)&remote, &addrlen);
        if (c_socket < 0) {
            close(c_socket);
            close(s_socket);
            perror(MESSAGE_ACCEPT_FAIL);
            return CODE_ACCEPT_FAIL;
        }
        std::cout << "Client " << inet_ntoa(remote.sin_addr)
                  << ":" << remote.sin_port << " connected" << std::endl;
        int code = accepted(c_socket);
        std::cout << "Client disconnected with code: " << code << std::endl;
    }
}

int udp_processing(int s_socket) {
    sockaddr_in remote;
    socklen_t addrlen = sizeof(remote);
    while (true) {
        auto r_len = recvfrom(s_socket, buffer, MAX_MESSAGE_SIZE, 0, (sockaddr*)&remote, &addrlen);
        if (r_len <= 0) {
            close(s_socket);
            perror(MESSAGE_RECEIVE_FAIL);
            return CODE_RECEIVE_FAIL;
        }
        std::cout << inet_ntoa(remote.sin_addr) << ":" << remote.sin_port << ": " << buffer << std::endl;
        parse(buffer);
        auto s_len = sendto(s_socket, buffer, strlen(buffer) + 1, 0, (sockaddr*)&remote, addrlen);
        if (s_len <= 0) {
            close(s_socket);
            perror(MESSAGE_SEND_FAIL);
            return CODE_SEND_FAIL;
        }
    }
}

// Start server for listening
int listen(const char* ip, unsigned short port, int protocol) {
    // Socket initialization
    int s_socket = socket(
            AF_INET,
            protocol == IPPROTO_TCP
            ? SOCK_STREAM
            : SOCK_DGRAM,
            protocol
    );
    if (s_socket == 0) {
        perror(MESSAGE_SOCKET_FAIL);
        return CODE_SOCKET_FAIL;
    }

    // Address initialization and binding
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    if (bind(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        close(s_socket);
        perror(MESSAGE_BIND_FAIL);
        return CODE_BIND_FAIL;
    }

    // Sever initialization (TCP only)
    if(IPPROTO_TCP == protocol && listen(s_socket, 1) < 0) {
        close(s_socket);
        perror(MESSAGE_LISTEN_FAIL);
        return CODE_LISTEN_FAIL;
    }
    std::cout << "Server on " << ip << ":" << port << " is listening..." << std::endl;
    std::cout << "Protocol: " << (IPPROTO_TCP == protocol ? "TCP" : "UDP") << std::endl;

    if(IPPROTO_TCP == protocol) {
        // TCP
        return tcp_processing(s_socket);
    }
    else {
        // UDP
        return udp_processing(s_socket);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return listen(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, IPPROTO_TCP);
    }
    if (argc == 2) {
        if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            std::cout << "Usage: server [address port] [option]" << std::endl;
            std::cout << "- address - ip-address for this server" << std::endl;
            std::cout << "- port - port for this server" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "--tcp - server must use TCP" << std::endl;
            std::cout << "--udp - server must use UDP" << std::endl;
            return CODE_SUCCESS;
        }
        if(strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
            return listen(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, IPPROTO_UDP);
        }
        if(strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
            return listen(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, IPPROTO_TCP);
        }
    }
    if (argc == 4) {
        if(strcmp(argv[3], "--udp") == 0)
            return listen(argv[1], atoi(argv[2]), IPPROTO_UDP);
        if(strcmp(argv[3], "--tcp") == 0)
            return listen(argv[1], atoi(argv[2]), IPPROTO_TCP);
    }
    std::cout << "Wrong parameters. Type 'server --help' for help" << std::endl;
    return CODE_SUCCESS;
}