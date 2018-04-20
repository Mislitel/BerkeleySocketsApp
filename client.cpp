#include "general.h"

char message[MAX_MESSAGE_SIZE];

int connect(const char* ip, unsigned short port, int protocol) {
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

    // Address initialization and connection
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    if (connect(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        close(s_socket);
        perror(MESSAGE_CONNECTION_FAIL);
        return CODE_CONNECTION_FAIL;
    }
    std::cout << "Connected on " << ip << ":" << port << std::endl;
    while(true) {
        std::cout << "c: ";
        std::cin.getline(message, MAX_MESSAGE_SIZE, '\n');
        // Message sending
        auto s_len = send(s_socket, message, strlen(message) + 1, 0);
        if(s_len <= 0) {
            close(s_socket);
            perror(MESSAGE_SEND_FAIL);
            return CODE_SEND_FAIL;
        }
        // If quit
        if(strcmp(message, COMMAND_QUIT) == 0) {
            close(s_socket);
            return CODE_SUCCESS;
        }
        // Answer receiving
        auto r_len = recv(s_socket, message, sizeof(message), 0);
        if(r_len <= 0) {
            close(s_socket);
            perror(MESSAGE_RECEIVE_FAIL);
            return CODE_RECEIVE_FAIL;
        }
        std::cout << "s: " << message << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, IPPROTO_TCP);
    }
    if (argc == 2) {
        if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            std::cout << "Usage: client [address port] [option]" << std::endl;
            std::cout << "- address - server's ip-address" << std::endl;
            std::cout << "- port - server's port" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "--tcp - connect as TCP-client" << std::endl;
            std::cout << "--udp - connect as UDP-client" << std::endl;
            return CODE_SUCCESS;
        }
        if(strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
            return connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, IPPROTO_UDP);
        }
        if(strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
            return connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT, IPPROTO_TCP);
        }
    }
    if (argc == 4) {
        if(strcmp(argv[3], "--udp") == 0)
            return connect(argv[1], atoi(argv[2]), IPPROTO_UDP);
        if(strcmp(argv[3], "--tcp") == 0)
            return connect(argv[1], atoi(argv[2]), IPPROTO_TCP);
    }
    std::cout << "Wrong parameters. Type 'client --help' for help" << std::endl;
    return CODE_SUCCESS;
}