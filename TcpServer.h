#ifndef BERKELEYSOCKETSAPP_TCPSERVER_H
#define BERKELEYSOCKETSAPP_TCPSERVER_H

#include "Server.h"

class TcpServer: public Server {
public:
    TcpServer() : TcpServer(DEFAULT_SERVER_IP, DEFAULT_TCP_PORT) {}
    TcpServer(const char* ip, unsigned short port);
    TcpServer(const TcpServer& source) : Server(source) {}

    void start() override;

private:
    void tcp_processing();
    void accepted(int c_socket, sockaddr_in remote);
};

#endif //BERKELEYSOCKETSAPP_SERVER_H
