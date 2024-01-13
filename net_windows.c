#include <stdio.h> // printf
#include <winsock2.h>
#include "net.h"

int net_port(unsigned short port)
{
    WSADATA wsa_data = {0};
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("failed to initialize winsock\n");
        return 0;
    }
    
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        printf("failed to create server socket\n");
        return 0;
    }
    
    u_long mode = 1; // 1 to enable non-blocking mode
    if (ioctlsocket(server, FIONBIO, &mode) == SOCKET_ERROR) {
        printf("failed to set server socket to nonblocking\n");
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
    
    return (int) server;
}

int net_sock(char *path)
{
    path = path;
    printf("to implement :)\n");
    return 0;
}

void net_listen(int server, net_handler *handler)
{
    #define max_clients 128
    static SOCKET clients[max_clients];
    static unsigned char read_buf[8192] = {0};
    
    if (!handler) {
        printf("net error: no socket request handler provided.\n");
        return;
    }
    
    for (int i = 0; i < max_clients; i++)
        clients[i] = INVALID_SOCKET;
    
    while (1) {
        fd_set readfds;
        fd_set writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(server, &readfds);
        
        for (int i = 0; i < max_clients; i++) {
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
                for (int i = 0; i < max_clients; i++) {
                    if (clients[i] == INVALID_SOCKET) {
                        // set the client socket to non-blocking mode
                        u_long client_mode = 1;
                        if (ioctlsocket(new_client, FIONBIO, &client_mode) == SOCKET_ERROR) {
                            printf("unable to set new connection as nonblocking.\n");
                            break;
                        }
                        
                        clients[i] = new_client;
                        handler((int) new_client, net_conn, 0, 0);
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
        
        for (int i = 0; i < max_clients; i++) {
            if (clients[i] == INVALID_SOCKET)
                continue;
            // read.
            if (FD_ISSET(clients[i], &readfds)) {
                while (1) {
                    int bytes_read = recv(clients[i], (char *) read_buf, sizeof(read_buf), 0);
                    if (bytes_read == SOCKET_ERROR) {
                        if (WSAGetLastError() != WSAEWOULDBLOCK) {
                            printf("failed while reading. closing the socket.\n");
                            handler((int) clients[i], net_closed, 0, 0);
                            closesocket(clients[i]);
                            clients[i] = INVALID_SOCKET;
                        }
                        break;
                    } else if (bytes_read == 0) {
                        // connection closed by client.
                        closesocket(clients[i]);
                        clients[i] = INVALID_SOCKET;
                        handler((int) clients[i], net_closed, 0, 0);
                        break;
                    } else {
                        // read.
                        handler((int) clients[i], net_read, read_buf, bytes_read);
                    }
                }
            }
            
            // write.
            if (FD_ISSET(clients[i], &writefds))
                handler((int) clients[i], net_write, 0, 0);
        }
    }
}

unsigned long long net_send(int socket, void *buf, unsigned long long n)
{
    unsigned long long sent = 0;
    if (!buf)
        return 0;
    while (sent < n) {
        int tmp = send((SOCKET) socket, (char *) buf + sent, (int) (n - sent), 0);
        if (tmp == SOCKET_ERROR) {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
                printf("net_write error\n");
            break;
        }
        sent += tmp;
    }
    return sent;
}

void net_close(int socket)
{
    closesocket(socket);
}
