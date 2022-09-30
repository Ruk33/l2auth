#ifndef ASYNC_SOCKET_H
#define ASYNC_SOCKET_H

#include <stddef.h> // size_t

enum asocket_event {
    ASOCKET_NEW_CONN,
    ASOCKET_CLOSED,
    ASOCKET_READ,
    ASOCKET_CAN_WRITE,
};

typedef void (asocket_handler)
(int socket, enum asocket_event event, void *read, size_t len);

// create a new socket for ipv4 assigned to a port.
int asocket_port(unsigned short port);
// create a new socket using a file (.sock, .socket, etc.)
int asocket_sock(char *path);
// block and listen for events.
void asocket_listen(int server, asocket_handler *handler);
// try to send the full buffer. returns the amount
// of bytes that was able to send. if it writes
// less than the intended amount, wait for ASOCKET_CAN_WRITE
// to keep sending past the bytes already sent.
size_t asocket_write(int socket, void *buf, size_t n);

#endif
