#include "Client.h"

Client::~Client() {
    if(connected)
        disconnect();
}

void Client::connectTo(const char *ip, unsigned short port, Protocol protocol) {
    if(isConnected())
        disconnect();

    // Socket initialization
    if(Protocol::TCP == protocol) {
        s_socket = socket(
                AF_INET,
                SOCK_STREAM,
                IPPROTO_TCP
        );
    } else {
        s_socket = socket(
                AF_INET,
                SOCK_DGRAM,
                IPPROTO_UDP
        );
    }
    if (s_socket == 0) {
        perror(MESSAGE_SOCKET_FAIL);
        throw std::runtime_error(MESSAGE_SOCKET_FAIL);
    }

    // Address initialization and connection
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
    if (connect(s_socket, (sockaddr*)&address, sizeof(address)) < 0) {
        close(s_socket);
        perror(MESSAGE_CONNECTION_FAIL);
        throw std::runtime_error(MESSAGE_CONNECTION_FAIL);
    }
    std::cout << "Connected to " << ip << ":" << port
              << ", protocol: " << (protocol == Protocol::TCP ? "TCP" : "UDP") << std::endl;
    connected = true;
}

void Client::disconnect() {
    close(s_socket);
    connected = false;
    std::cout << "Disconnected from " << inet_ntoa(address.sin_addr) << ":" << htons(address.sin_port) << std::endl;
}

void Client::cycleSendReceive() {
    if (!connected) {
        std::cerr << "No connection!" << std::endl;
    }

    while(true) {
        std::cout << "c: ";
        std::cin.getline(message, MAX_MESSAGE_SIZE, '\n');
        // Message sending
        auto s_len = send(s_socket, message, strlen(message) + 1, 0);
        if(s_len <= 0) {
            close(s_socket);
            perror(MESSAGE_SEND_FAIL);
            throw std::runtime_error(MESSAGE_SEND_FAIL);
        }
        // If quit
        if(strcmp(message, COMMAND_QUIT) == 0) {
            disconnect();
            return;
        }
        // Answer receiving
        auto r_len = recv(s_socket, message, sizeof(message), 0);
        if(r_len <= 0) {
            close(s_socket);
            perror(MESSAGE_RECEIVE_FAIL);
            throw std::runtime_error(MESSAGE_RECEIVE_FAIL);
        }
        std::cout << "s: " << message << std::endl;
    }
}




int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            Client client;
            client.connectTo(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT, Protocol::TCP);
            client.cycleSendReceive();
            return 0;
        }
        if (argc == 2) {
            if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
                std::cout << "Usage:" << std::endl;
                std::cout << " client [option]" << std::endl;
                std::cout << " client [option] <ip> <port>" << std::endl;
                std::cout << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << " -h, --help\tdisplay this help and exit" << std::endl;
                std::cout << " -t, --tcp\tuse TCP for data transfer" << std::endl;
                std::cout << " -u, --udp\tuse UDP for data transfer" << std::endl;
                return 0;
            }
            if (strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
                Client client;
                client.connectTo(DEFAULT_SERVER_IP, DEFAULT_UDP_PORT, Protocol::UDP);
                client.cycleSendReceive();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
                Client client;
                client.connectTo(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT, Protocol::TCP);
                client.cycleSendReceive();
                return 0;
            }
        }
        if (argc == 3) {
            Client client;
            client.connectTo(argv[1], atoi(argv[2]), Protocol::TCP);
            client.cycleSendReceive();
            return 0;
        }
        if (argc == 4) {
            if (strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
                Client client;
                client.connectTo(argv[2], atoi(argv[3]), Protocol::UDP);
                client.cycleSendReceive();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
                Client client;
                client.connectTo(argv[2], atoi(argv[3]), Protocol::TCP);
                client.cycleSendReceive();
                return 0;
            }
        }
        std::cout << "Wrong parameters. Type 'client --help' for help" << std::endl;
        return 0;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}