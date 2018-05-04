#ifndef BERKELEYSOCKETSAPP_CLIENT_H
#define BERKELEYSOCKETSAPP_CLIENT_H

#include "general.h"

class Client {
public:
    Client() : s_socket(0), connected(false) {}
    ~Client();
    bool isConnected() { return connected; }

    virtual void connectTo(const char* ip, unsigned short port) = 0;
    virtual void disconnect();

    virtual void singleSendReceive(const char* message) = 0;
    void cycleSendReceive();

protected:
    int s_socket;
    char message[MAX_MESSAGE_SIZE] = {'\0'};
    bool connected;
};

#endif //BERKELEYSOCKETSAPP_CLIENT_H