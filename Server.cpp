#include "Server.h"
#include "TcpServer.h"
#include "UdpServer.h"

std::mutex Server::mtx;

Server::Server(const char *ip, unsigned short port) {
    // Address initialization and binding
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




int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            TcpServer tcp_server;
            tcp_server.start();
        }
        if (argc == 2) {
            if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
                std::cout << "Usage:" << std::endl;
                std::cout << " server [option]" << std::endl;
                std::cout << " server [--tcp | --udp] <ip> <port>" << std::endl;
                std::cout << " server --tcp | --udp <ip> <port> --tcp | --udp <ip> <port>" << std::endl;
                std::cout << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << " -b, --both\tuse TCP and UDP for data transfer for default settings" << std::endl;
                std::cout << " -h, --help\tdisplay this help and exit" << std::endl;
                std::cout << " -t, --tcp\tuse TCP for data transfer" << std::endl;
                std::cout << " -u, --udp\tuse UDP for data transfer" << std::endl;
                std::cout << std::endl;
                std::cout << "Examples:" << std::endl;
                std::cout << " server\t\t\t\t\t\t\t\t\t\t\t\tlaunches server TCP on 127.0.0.1:6111" << std::endl;
                std::cout << " server --udp\t\t\t\t\t\t\t\t\t\tlaunches server UDP on 127.0.0.1:4111" << std::endl;
                std::cout << " server --both\t\t\t\t\t\t\t\t\t\tlaunches server TCP on 127.0.0.1:6111 and server UDP on 127.0.0.1:4111" << std::endl;
                std::cout << " server --tcp 127.0.0.1 1234\t\t\t\t\t\t\tlaunches server TCP on 127.0.0.1:1234" << std::endl;
                std::cout << " server --tcp 127.0.0.1 1234 --udp 127.0.0.1 3456\tlaunches server TCP on 127.0.0.1:1234 and server UDP on 127.0.0.1:3456" << std::endl;
                return 0;
            }
            if (strcmp(argv[1], "--udp") == 0 || strcmp(argv[1], "-u") == 0) {
                UdpServer udpServer;
                udpServer.start();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0 || strcmp(argv[1], "-t") == 0) {
                TcpServer tcpServer;
                tcpServer.start();
                return 0;
            }
            if (strcmp(argv[1], "--both") == 0 || strcmp(argv[1], "-b") == 0) {
                TcpServer tcp_server;
                std::thread tcp_thread(&TcpServer::start, tcp_server);
                UdpServer udp_server;
                std::thread udp_thread(&UdpServer::start, udp_server);
                udp_thread.join();
                tcp_thread.join();
                return 0;
            }
        }
        if (argc == 4) {
            if (strcmp(argv[1], "--udp") == 0) {
                UdpServer udpServer(argv[2], atoi(argv[3]));
                udpServer.start();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0) {
                TcpServer tcpServer(argv[2], atoi(argv[3]));
                tcpServer.start();
                return 0;
            }
        }
        if (argc == 7) {
            if (strcmp(argv[1], "--udp") == 0 && strcmp(argv[4], "--tcp") == 0) {
                UdpServer udp_server(argv[2], atoi(argv[3]));
                TcpServer tcp_server(argv[5], atoi(argv[6]));
                std::thread udp_thread(&UdpServer::start, udp_server);
                std::thread tcp_thread(&TcpServer::start, tcp_server);
                udp_thread.join();
                tcp_thread.join();
                return 0;
            }
            if (strcmp(argv[1], "--tcp") == 0 && strcmp(argv[4], "--udp") == 0) {
                TcpServer tcp_server(argv[2], atoi(argv[3]));
                UdpServer udp_server(argv[5], atoi(argv[6]));
                std::thread tcp_thread(&TcpServer::start, tcp_server);
                std::thread udp_thread(&UdpServer::start, udp_server);
                udp_thread.join();
                tcp_thread.join();
                return 0;
            }
        }
        std::cout << "Wrong parameters. Type 'server --help' for help" << std::endl;
        return 0;
    }
    catch (std::runtime_error& e) {
        perror(e.what());
        return 1;
    }
}

