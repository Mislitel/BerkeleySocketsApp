#ifndef BERKELEYSOCKETSAPP_SERVER_H
#define BERKELEYSOCKETSAPP_SERVER_H

#include "general.h"

class Server {
public:
    Server() {}
    Server(const char* ip, unsigned short port);
    Server(const Server& source);
    ~Server();

    virtual void start() = 0;

protected:
    char buffer[MAX_MESSAGE_SIZE];
    int s_socket;
    sockaddr_in address;
    static std::mutex mtx;
};


#endif //BERKELEYSOCKETSAPP_SERVER_H
