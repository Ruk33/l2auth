#ifndef ASYNC_SOCKET_H
#define ASYNC_SOCKET_H

#include "l2auth.h"

struct ipv4 {
    char buf[sizeof("255.255.255.255")];
};

enum network_event {
    NETWORK_NEW_CONN,
    NETWORK_CLOSED,
    NETWORK_READ,
    NETWORK_CAN_WRITE,
};

typedef void (network_handler)
(int socket, enum network_event event, void *read, size_t len);

// create a new socket for ipv4 assigned to a port.
int network_port(unsigned short port);
// create a new socket using a file (.sock, .socket, etc.)
int network_sock(char *path);
// block and listen for events.
void network_listen(int server, network_handler *handler);
// try to send the full buffer. returns the amount
// of bytes that was able to send. if it writes
// less than the intended amount, wait for NETWORK_CAN_WRITE
// to keep sending past the bytes already sent.
size_t network_write(int socket, void *buf, size_t n);
// 0 if fails
// 1 if succeeds.
int network_ipv4_to_u32(u32 *dest, struct ipv4 *src);

#endif
