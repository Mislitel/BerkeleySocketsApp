#include "TcpTools.h"

ssize_t TcpTools::receiveMessage(int socket, void *buffer, uint32_t buffer_size) {
    // Data size receiving
    uint32_t len;
    ssize_t r = receive_packet(socket, &len, sizeof(len));
    if (r <= 0)
        return r;
    // Data receiving
    r = receive_packet(socket, buffer, len > buffer_size
                                       ? buffer_size
                                       : len
    );
    return r;
}

ssize_t TcpTools::sendMessage(int socket, const void *buffer, uint32_t buffer_size) {
    // Data size sending
    ssize_t r = send_packet(socket, &buffer_size, sizeof(buffer_size));
    if (r <= 0)
        return r;
    // Data sending
    r = send_packet(socket, buffer, buffer_size);
    return r;
}

// Packet receiving
ssize_t TcpTools::receive_packet(int socket, void *buffer, uint32_t buffer_size) {
    ssize_t received = 0;
    while (buffer_size > 0) {
        ssize_t len = recv(socket, buffer, buffer_size, 0);
        // If syscall failure - try again
        if (errno == EINTR)
            continue;
        if (len <= 0)
            return len;
        buffer_size -= len;
        received += len;
        buffer = static_cast<char *>(buffer) + len;
    }
    return received;
}

// Packet sending
ssize_t TcpTools::send_packet(int socket, const void *buffer, uint32_t buffer_size) {
    ssize_t sended = 0;
    while (buffer_size > 0) {
        ssize_t len = send(socket, buffer, buffer_size, 0);
        // If syscall failure - try again
        if (errno == EINTR)
            continue;
        if (len <= 0)
            return len;
        buffer_size -= len;
        sended += len;
        buffer = static_cast<const char *>(buffer) + len;
    }
    return sended;
}