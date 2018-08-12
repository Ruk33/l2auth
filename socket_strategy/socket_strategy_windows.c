#ifndef L2AUTH_SOCKET_STRATEGY_WINDOWS_C
#define L2AUTH_SOCKET_STRATEGY_WINDOWS_C

#include <windows.h>
#include <winsock2.h>
#include <core/l2_socket.c>

int socket_strategy_windows_connect(struct l2_socket* l2_socket)
{
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa)!=0) {
        return L2_SOCKET_WIN_WSA_STARTUP_ERROR;
    }

    l2_socket->socket = (void *) socket(AF_INET, SOCK_STREAM, 0);

    if ((SOCKET) l2_socket->socket==INVALID_SOCKET) {
        return L2_SOCKET_WIN_CREATE_SOCKET_ERROR;
    }

    return 0;
}

int socket_strategy_windows_bind(struct l2_socket* l2_socket, unsigned short port)
{
    SOCKADDR_IN addr;

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind((SOCKET) l2_socket->socket, (SOCKADDR*) &addr, sizeof(addr))==SOCKET_ERROR) {
        return L2_SOCKET_WIN_BIND_ERROR;
    }

    return 0;
}

int socket_strategy_windows_listen(struct l2_socket* l2_socket)
{
    if (listen((SOCKET) l2_socket->socket, 3)!=0) {
        return L2_SOCKET_WIN_LISTEN_ERROR;
    }

    return 0;
}

int socket_strategy_windows_accept(struct l2_socket* server, struct l2_socket* client)
{
    SOCKADDR_IN addr;
    int addr_size = sizeof(addr);

    client->socket = (void *) accept((SOCKET) server->socket, (struct sockaddr*) &addr, &addr_size);

    if ((SOCKET) client->socket==INVALID_SOCKET) {
        return L2_SOCKET_WIN_ACCEPT_ERROR;
    }

    return 0;
}

int socket_strategy_windows_receive(struct l2_socket* l2_socket, char* buffer, int buffer_size)
{
    return recv((SOCKET) l2_socket->socket, buffer, buffer_size, 0);
}

int socket_strategy_windows_send(struct l2_socket* l2_socket, char* buffer, int buffer_size)
{
    return send((SOCKET) l2_socket->socket, buffer, buffer_size, 0);
}

void socket_strategy_windows(struct l2_socket_strategy* socket_strategy)
{
    socket_strategy->connect = socket_strategy_windows_connect;
    socket_strategy->bind = socket_strategy_windows_bind;
    socket_strategy->listen = socket_strategy_windows_listen;
    socket_strategy->accept = socket_strategy_windows_accept;
    socket_strategy->receive = socket_strategy_windows_receive;
    socket_strategy->send = socket_strategy_windows_send;
}

#endif //L2AUTH_SOCKET_STRATEGY_WINDOWS_C