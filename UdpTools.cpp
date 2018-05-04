#include "UdpTools.h"

ssize_t UdpTools::sendMessage(int socket, const void *buffer, uint32_t buffer_size) {
    return sendto(socket, buffer, buffer_size, 0, (sockaddr*)&remote, addrlen);
}

ssize_t UdpTools::receiveMessage(int socket, void *buffer, uint32_t buffer_size) {
    return recvfrom(socket, buffer, buffer_size, 0, (sockaddr*)&remote, &addrlen);
}