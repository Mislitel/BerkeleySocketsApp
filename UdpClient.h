#ifndef BERKELEYSOCKETSAPP_UDPCLIENT_H
#define BERKELEYSOCKETSAPP_UDPCLIENT_H

#include "Client.h"
#include "UdpTools.h"

class UdpClient : public Client, public UdpTools {
public:
    void connectTo(const char* ip, unsigned short port) override;
    void singleSendReceive(const char* message) override;

    void disconnect() override;
};


#endif //BERKELEYSOCKETSAPP_UDPCLIENT_H