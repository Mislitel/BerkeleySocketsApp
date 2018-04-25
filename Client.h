#ifndef BERKELEYSOCKETSAPP_CLIENT_H
#define BERKELEYSOCKETSAPP_CLIENT_H

#include "general.h"

enum class Protocol { TCP, UDP };

class Client {
public:
    Client() : connected(false) {}
    Client(const Client& source) : s_socket(source.s_socket), connected(source.connected) {}
    ~Client();
    bool isConnected() { return connected; }

    void connectTo(const char* ip, unsigned short port, Protocol protocol);
    void disconnect();

    void cycleSendReceive();

private:
    int s_socket;
    sockaddr_in address;
    char message[MAX_MESSAGE_SIZE];
    bool connected;
};

#endif //BERKELEYSOCKETSAPP_CLIENT_H