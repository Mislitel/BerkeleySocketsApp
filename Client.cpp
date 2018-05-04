#include "Client.h"

Client::~Client() {
    if(connected)
        disconnect();
}

void Client::disconnect() {
    close(s_socket);
    connected = false;
}

void Client::cycleSendReceive() {
    while(true) {
        std::cout << "c: ";
        std::cin.getline(message, MAX_MESSAGE_SIZE, '\n');
        std::cin.clear();
        singleSendReceive(message);
        if(!connected)
            return;
    }
}