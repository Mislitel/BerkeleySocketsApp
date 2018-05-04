#include "TcpClient.h"

void TcpClient::disconnect() {
    Client::disconnect();
    std::cout << "Disconnected from " << inet_ntoa(address.sin_addr)
              << ":" << htons(address.sin_port) << std::endl;
}

void TcpClient::connectTo(const char *ip, unsigned short port) {
    if(isConnected())
        disconnect();

    s_socket = socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
    );
    if (s_socket < 0) {
        perror(SUBJECT_CLIENT);
        throw std::runtime_error(MESSAGE_SOCKET_FAIL);
    }

    // Address initialization and connection
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    if (connect(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        close(s_socket);
        perror(SUBJECT_CLIENT);
        throw std::runtime_error(MESSAGE_CONNECTION_FAIL);
    }
    std::cout << "Connected to " << ip << ":" << port
              << ", protocol: TCP" << std::endl;
    connected = true;
}

void TcpClient::singleSendReceive(const char *msg) {
    if (!connected) {
        std::cerr << "No connection!" << std::endl;
        return;
    }

    ssize_t s_len = sendMessage(s_socket, msg, strlen(msg) + 1);
    if (s_len == 0) {
        std::cerr << "Server closed connection." << std::endl;
        disconnect();
        return;
    }
    if (s_len < 0) {
        perror(MESSAGE_SEND_FAIL);
        disconnect();
        return;
    }
    // If quit
    if (strcmp(msg, COMMAND_QUIT) == 0) {
        disconnect();
        return;
    }
    // Receiving message
    ssize_t r_len = receiveMessage(s_socket, message, MAX_MESSAGE_SIZE);
    if (r_len == 0) {
        std::cerr << "Server closed connection." << std::endl;
        disconnect();
        return;
    }
    if (r_len < 0) {
        perror(MESSAGE_RECEIVE_FAIL);
        disconnect();
        return;
    }
    std::cout << "s: " << message << std::endl;
}