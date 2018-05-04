#ifndef BERKELEYSOCKETSAPP_NETWORKTOOLS_H
#define BERKELEYSOCKETSAPP_NETWORKTOOLS_H

#include "general.h"

class NetworkTools {
protected:
    virtual ssize_t receiveMessage(int socket, void *buffer, uint32_t buffer_size) = 0;
    virtual ssize_t sendMessage(int socket, const void *buffer, uint32_t buffer_size) = 0;
};


#endif //BERKELEYSOCKETSAPP_NETWORKTOOLS_H
