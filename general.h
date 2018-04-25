#ifndef BERKELEYSOCKETSAPP_GENERAL_H
#define BERKELEYSOCKETSAPP_GENERAL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "Parser.h"

#define DEFAULT_SERVER_IP           "127.0.0.1"
#define DEFAULT_TCP_PORT            6111
#define DEFAULT_UDP_PORT            4111

#define MAX_MESSAGE_SIZE            65536

#define MESSAGE_SOCKET_FAIL         "Fail to create the socket"
#define MESSAGE_BIND_FAIL           "Fail to bind"
#define MESSAGE_LISTEN_FAIL         "Fail to listen"
#define MESSAGE_ACCEPT_FAIL         "Fail to accept"
#define MESSAGE_RECEIVE_FAIL        "Fail to receive"
#define MESSAGE_SEND_FAIL           "Fail to send"
#define MESSAGE_CONNECTION_FAIL     "Fail to connect"
#define MESSAGE_WRONG_PROTOCOL      "Wrong protocol"

#define SUBJECT_TCP_SERVER          "Server TCP"
#define SUBJECT_UDP_SERVER          "Server UDP"
#define SUBJECT_CLIENT              "Client"

#define COMMAND_QUIT                "quit"

#endif //BERKELEYSOCKETSAPP_GENERAL_H
