#include <stdio.h> // printf
#include <winsock2.h>
#include "asocket.h"

int asocket_port(unsigned short port)
{
    WSADATA wsa_data = {0};
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("failed to initialize Winsock\n");
        return 0;
    }
    
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        printf("failed to create server socket\n");
        return 0;
    }
    
    u_long mode = 1; // 1 to enable non-blocking mode
    if (ioctlsocket(server, FIONBIO, &mode) == SOCKET_ERROR) {
        printf("failed to set server socket to nonblockingt\n");
        return 0;
    }
    
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    
    if (bind(server, (struct sockaddr*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("failed to bind server socket\n");
        closesocket(server);
        return 0;
    }
    
    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        printf("failed to listen on server socket\n");
        closesocket(server);
        return 0;
    }
    
    return server;
}

int asocket_sock(char *path)
{
    printf("to implement :)\n");
    return 0;
}

void asocket_listen(int server, asocket_handler *handler)
{
#define MAX_CLIENTS 128
#define MAX_BUF_SIZE 8192
    
    if (!handler) {
        printf("asocket error: no socket request handler provided.\n");
        return;
    }
    
    SOCKET clients[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++)
        clients[i] = INVALID_SOCKET;
    
    while (1) {
        fd_set readfds;
        fd_set writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(server, &readfds);
        
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == INVALID_SOCKET)
                continue;
            // check it's a valid socket before adding it to
            // the set, otherwise select will fail.
            int optval = 0;
            int optlen = sizeof(optval);
            if (getsockopt(clients[i], SOL_SOCKET, SO_ERROR, (char*) &optval, &optlen) != SOCKET_ERROR && optval == 0) {
                FD_SET(clients[i], &readfds);
                FD_SET(clients[i], &writefds);
            } else {
                clients[i] = INVALID_SOCKET; 
            }
        }
        
        int activity = select(0, &readfds, &writefds, 0, 0);
        if (activity == SOCKET_ERROR) {
            printf("select error %d\n", WSAGetLastError());
            break;
        }
        
        // accept new connections.
        if (FD_ISSET(server, &readfds)) {
            while (1) {
                SOCKET new_client = accept(server, 0, 0);
                if (new_client == INVALID_SOCKET)
                    break;
                int accepted = 0;
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (clients[i] == INVALID_SOCKET) {
                        // set the client socket to non-blocking mode
                        u_long client_mode = 1;
                        if (ioctlsocket(new_client, FIONBIO, &client_mode) == SOCKET_ERROR) {
                            printf("unable to set new connection as nonblocking.\n");
                            break;
                        }
                        
                        clients[i] = new_client;
                        handler(new_client, ASOCKET_NEW_CONN, 0, 0);
                        accepted = 1;
                        break;
                    }
                }
                if (!accepted) {
                    printf("can't accept new connection, maximum reached.\n");
                    closesocket(new_client);
                }
            }
        }
        
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == INVALID_SOCKET)
                continue;
            // read.
            if (FD_ISSET(clients[i], &readfds)) {
                while (1) {
                    char buffer[MAX_BUF_SIZE] = {0};
                    int bytes_read = recv(clients[i], buffer, sizeof(buffer), 0);
                    if (bytes_read == SOCKET_ERROR) {
                        if (WSAGetLastError() != WSAEWOULDBLOCK)
                            printf("failed while reading.\n");
                        break;
                    } else if (bytes_read == 0) {
                        // connection closed by client.
                        closesocket(clients[i]);
                        clients[i] = INVALID_SOCKET;
                        handler(clients[i], ASOCKET_CLOSED, 0, 0);
                        break;
                    } else {
                        // read.
                        handler(clients[i], ASOCKET_READ, buffer, bytes_read);
                    }
                }
            }
            
            // write.
            if (FD_ISSET(clients[i], &writefds))
                handler(clients[i], ASOCKET_CAN_WRITE, 0, 0);
        }
    }
}

unsigned long long asocket_write(int socket, void *buf, unsigned long long n)
{
    unsigned long long sent = 0;
    if (!buf)
        return 0;
    while (sent < n) {
        int tmp = send(socket, (char *) buf + sent, n - sent, 0);
        if (tmp == SOCKET_ERROR) {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
                printf("asocket_write error\n");
            break;
        }
        sent += tmp;
    }
    return sent;
}

void asocket_close(int socket)
{
    closesocket(socket);
}
