#ifndef BERKELEYSOCKETSAPP_UDPSERVER_H
#define BERKELEYSOCKETSAPP_UDPSERVER_H

#include "Server.h"

class UdpServer: public Server {
public:
    UdpServer() : UdpServer(DEFAULT_SERVER_IP, DEFAULT_UDP_PORT) {}
    UdpServer(const char* ip, unsigned short port);
    UdpServer(const UdpServer& source) : Server(source) {}

    void start() override;

private:
    void udp_processing();
};


#endif //BERKELEYSOCKETSAPP_UDPSERVER_H
