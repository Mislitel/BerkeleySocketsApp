#ifndef BERKELEYSOCKETSAPP_TCPTOOLS_H
#define BERKELEYSOCKETSAPP_TCPTOOLS_H

#include "NetworkTools.h"

class TcpTools : public NetworkTools {
protected:
    ssize_t sendMessage(int socket, const void* buffer, uint32_t buffer_size) override;
    ssize_t receiveMessage(int socket, void* buffer, uint32_t buffer_size) override;

private:
    ssize_t send_packet(int socket, const void* buffer, uint32_t buffer_size);
    ssize_t receive_packet(int socket, void* buffer, uint32_t buffer_size);
};


#endif //BERKELEYSOCKETSAPP_TCPTOOLS_H
