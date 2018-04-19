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

#include "parser.h"

#define DEFAULT_SERVER_IP           "127.0.0.1"
#define DEFAULT_SERVER_PORT         6111

#define MAX_MESSAGE_SIZE            65536

#define CODE_SUCCESS                0x0
#define CODE_SOCKET_FAIL            0x1
#define CODE_BIND_FAIL              0x2
#define CODE_LISTEN_FAIL            0x3
#define CODE_ACCEPT_FAIL            0x4
#define CODE_RECEIVE_FAIL           0x5
#define CODE_SEND_FAIL              0x6
#define CODE_CONNECTION_FAIL        0x7

#define MESSAGE_SOCKET_FAIL         "Fail to create the socket"
#define MESSAGE_BIND_FAIL           "Fail to bind"
#define MESSAGE_LISTEN_FAIL         "Fail to listen"
#define MESSAGE_ACCEPT_FAIL         "Fail to accept"
#define MESSAGE_RECEIVE_FAIL        "Fail to receive"
#define MESSAGE_SEND_FAIL           "Fail to send"
#define MESSAGE_CONNECTION_FAIL     "Fail to connect"

#define COMMAND_QUIT                "quit"

#endif //BERKELEYSOCKETSAPP_GENERAL_H
