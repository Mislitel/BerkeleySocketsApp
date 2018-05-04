#ifndef BERKELEYSOCKETSAPP_TCPCLIENT_H
#define BERKELEYSOCKETSAPP_TCPCLIENT_H

#include "Client.h"
#include "TcpTools.h"

class TcpClient : public Client, public TcpTools {
public:
    void connectTo(const char* ip, unsigned short port) override;
    void singleSendReceive(const char* message) override;

    void disconnect() override;

private:
    sockaddr_in address;
};


#endif //BERKELEYSOCKETSAPP_TCPCLIENT_H
