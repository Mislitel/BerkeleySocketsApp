#ifndef BERKELEYSOCKETSAPP_UDPTOOLS_H
#define BERKELEYSOCKETSAPP_UDPTOOLS_H

#include "NetworkTools.h"

class UdpTools : public NetworkTools {
protected:
    sockaddr_in remote;
    socklen_t addrlen = sizeof(remote);
    ssize_t sendMessage(int socket, const void* buffer, uint32_t buffer_size) override;
    ssize_t receiveMessage(int socket, void* buffer, uint32_t buffer_size) override;
};


#endif //BERKELEYSOCKETSAPP_UDPTOOLS_H
