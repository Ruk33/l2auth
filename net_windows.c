#include "net.h"

int net_set_non_blocking(SOCKET socket)
{
    u_long non_blocking_mode = 1;

    if (ioctlsocket(socket, FIONBIO, &non_blocking_mode) == SOCKET_ERROR) {
        fprintf(stderr, "failed to set socket to nonblocking\n");
        return 0;
    }

    return 1;
}

struct net_socket net_port(unsigned short port)
{
    struct net_socket result = {0};

    result.handle = INVALID_SOCKET;

    WSADATA wsa_data = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        fprintf(stderr, "failed to initialize winsock\n");
        return result;
    }

    result.handle = socket(AF_INET, SOCK_STREAM, 0);

    if (result.handle == INVALID_SOCKET) {
        fprintf(stderr, "failed to create server socket\n");
        return result;
    }

    if (!net_set_non_blocking(result.handle)) {
        fprintf(stderr, "failed to set socket as non blocking\n");
        return result;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (bind(result.handle, (struct sockaddr*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "failed to bind server socket\n");
        closesocket(result.handle);
        return result;
    }

    if (listen(result.handle, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(stderr, "failed to listen on server socket\n");
        closesocket(result.handle);
        return result;
    }

    return result;
}

void net_block_and_listen(struct net_socket server, net_handler *handler)
{
    #define max_clients 1024
    static SOCKET clients[max_clients];
    static unsigned char read_buf[8192] = {0};

    SOCKET highest = 0;

    if (!handler) {
        fprintf(stderr, "net error: no socket request handler provided.\n");
        return;
    }

    for (int i = 0; i < max_clients; i++)
        clients[i] = INVALID_SOCKET;

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server.handle, &readfds);

        highest = server.handle;

        for (int i = 0; i < max_clients; i++) {
            if (clients[i] == INVALID_SOCKET)
                continue;
            /*
             * Check it's a valid socket before adding it to
             * the set, otherwise select will fail.
             */
            int optval = 0;
            int optlen = sizeof(optval);
            if (getsockopt(clients[i], SOL_SOCKET, SO_ERROR, (char*) &optval, &optlen) != SOCKET_ERROR && optval == 0) {
                FD_SET(clients[i], &readfds);
                if (clients[i] > highest)
                    highest = clients[i];
            } else {
                clients[i] = INVALID_SOCKET;
            }
        }

        int activity = select((highest + 1), &readfds, 0, 0, 0);
        if (activity == SOCKET_ERROR) {
            fprintf(stderr, "select error %d\n", WSAGetLastError());
            break;
        }

        /*
         * Accept new connections.
         */
        if (FD_ISSET(server.handle, &readfds)) {
            while (1) {
                SOCKET new_client = accept(server.handle, 0, 0);

                if (new_client == INVALID_SOCKET)
                    break;

                int accepted = 0;

                for (int i = 0; i < max_clients; i++) {
                    if (clients[i] == INVALID_SOCKET) {
                        if (!net_set_non_blocking(new_client))
                            break;

                        clients[i] = new_client;

                        struct net_socket socket = {0};

                        socket.handle = new_client;

                        handler(socket, net_conn, 0, 0);

                        accepted = 1;
                        break;
                    }
                }

                if (!accepted) {
                    fprintf(stderr, "can't accept new connection, maximum reached.\n");
                    closesocket(new_client);
                }
            }
        }

        for (int i = 0; i < max_clients; i++) {
            if (clients[i] == INVALID_SOCKET)
                continue;
            /*
             * Read
             */
            if (FD_ISSET(clients[i], &readfds)) {
                while (1) {
                    struct net_socket socket = {0};

                    socket.handle = clients[i];

                    int bytes_read = recv(clients[i], (char *) read_buf, sizeof(read_buf), 0);

                    if (bytes_read == SOCKET_ERROR) {
                        if (WSAGetLastError() != WSAEWOULDBLOCK) {
                            fprintf(stderr, "failed while reading. closing the socket.\n");

                            handler(socket, net_closed, 0, 0);

                            closesocket(clients[i]);
                            clients[i] = INVALID_SOCKET;
                        }
                        break;
                    } else if (bytes_read == 0) {
                        handler(socket, net_closed, 0, 0);

                        /*
                         * Connection closed by the client.
                         */
                        closesocket(clients[i]);
                        clients[i] = INVALID_SOCKET;

                        break;
                    } else {
                        /*
                         * Read
                         */
                        handler(socket, net_read, read_buf, bytes_read);
                    }
                }
            }
        }
    }
}

int net_send(struct net_socket socket, void *buf, int n)
{
    int sent = 0;

    if (!buf)
        return 0;

    while (sent < n) {
        int tmp = send(socket.handle, (char *) buf + sent, n - sent, 0);
        if (tmp == SOCKET_ERROR) {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
                fprintf(stderr, "net_write error\n");
            break;
        }
        sent += tmp;
    }

    return sent;
}

void net_close(struct net_socket socket)
{
    closesocket(socket.handle);
}
