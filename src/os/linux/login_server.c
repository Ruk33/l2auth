#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "../../server/login/include/client.h"
#include "../../server/login/include/state.h"
#include "../../server/login/include/login.h"

#include "../../util.c"
#include "../../packet.c"
#include "../../server/login/state.c"
#include "../../server/login/client.c"
#include "../../server/login/client_packet.c"
#include "../../server/login/server_packet.c"
#include "../../server/login/login.c"

#define MAX_SOCKETS 512

struct unix_socket {
    int fd;
    int used;
    byte buf_write[32768];
    size_t sent;
    size_t write_size;
    time_t last_packet_received_at;
    struct client *client;
};

static int g_epoll_fd = 0;
static struct unix_socket g_sockets[MAX_SOCKETS] = { 0 };
static struct state g_state = { 0 };

static void unix_socket_set_non_block(int fd)
{
    int flags = 0;

    flags = fcntl(fd, F_GETFL, 0);
    flags = flags < 0 ? 0 : flags;

    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int unix_socket_init(struct unix_socket *src, u16 port, int max)
{
    struct sockaddr_in address = { 0 };
    struct sockaddr *address_p = 0;

    int reusable_enable = 0;

    src->fd = socket(AF_INET, SOCK_STREAM, 0);
    src->used = 1;

    if (src->fd < 0) {
        goto abort;
    }

    unix_socket_set_non_block(src->fd);

    // Make the socket reusable.
    reusable_enable = 1;
    if (setsockopt(src->fd, SOL_SOCKET, SO_REUSEADDR, &reusable_enable, sizeof(int)) < 0) {
        goto abort;
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(port);

    address_p = (struct sockaddr *) &address;

    if (bind(src->fd, address_p, sizeof(address)) < 0) {
        goto abort;
    }

    if (listen(src->fd, max) < 0) {
        goto abort;
    }

    return 1;

abort:
    close(src->fd);
    *src = (struct unix_socket){ 0 };

    return 0;
}

static int unix_socket_accept(struct unix_socket *dest, struct unix_socket *src)
{
    struct sockaddr_in address = { 0 };
    struct sockaddr *address_p = 0;
    socklen_t addrlen = 0;

    struct epoll_event event = { 0 };

    if (!dest) {
        printf("unable to accept new client. too many clients?\n");
        return 0;
    }

    address_p = (struct sockaddr *) &address;

    dest->fd = accept(src->fd, address_p, &addrlen);
    dest->used = 1;
    dest->last_packet_received_at = time(0);

    if (dest->fd < 0) {
        *dest = (struct unix_socket){ 0 };
        printf("failed to accept new client.\n");
        return 0;
    }

    unix_socket_set_non_block(dest->fd);

    event.events = EPOLLIN | EPOLLET;
    event.data.ptr = dest;

    return epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, dest->fd, &event) != -1;
}

static void unix_socket_close(struct unix_socket *socket)
{
    epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, socket->fd, 0);
    shutdown(socket->fd, SHUT_RDWR);
    close(socket->fd);
    *socket = (struct unix_socket){ 0 };
}

static struct unix_socket *find_free_socket(void)
{
    for (size_t i = 0; i < ARR_LEN(g_sockets); i += 1) {
        if (!g_sockets[i].used) {
            return &g_sockets[i];
        }
    }
    return 0;
}

static void unix_socket_flush(struct unix_socket *socket)
{
    ssize_t sent = 0;

    assert(socket);

    if (!socket->write_size) {
        return;
    }

    sent = send(
        socket->fd,
        socket->buf_write + socket->sent,
        socket->write_size - socket->sent,
        0
    );

    socket->sent += MAX(0, sent);

    // If the entire packet has been sent, reset the counter.
    if (socket->write_size == socket->sent) {
        socket->write_size = 0;
        socket->sent = 0;
    }
}

static void unix_socket_write(struct unix_socket *socket, byte *buf, size_t n)
{
    assert(socket);
    assert(buf);

    // Append buf to the end of the write buffer.
    memcpy(socket->buf_write + socket->write_size, buf, n);
    socket->write_size += n;
}

static int unix_socket_listen(struct unix_socket *server)
{
    static struct epoll_event events[MAX_SOCKETS] = { 0 };
    struct epoll_event event = { 0 };
    int ev_count = 0;

    struct unix_socket *socket = 0;
    ssize_t read = 0;

    time_t now = 0;

    g_epoll_fd = epoll_create1(0);

    if (g_epoll_fd < 0) {
        printf("unable to create epoll.\n");
        return 0;
    }

    event.events = EPOLLIN;
    event.data.ptr = server;

    epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, server->fd, &event);
    printf("listening for new connections.\n");

    while (1) {
        ev_count = epoll_wait(g_epoll_fd, events, ARR_LEN(events), -1);

        if (ev_count < 0) {
            printf("epoll_wait failed, closing the server.\n");
            close(g_epoll_fd);
            return 0;
        }

        now = time(0);

        // Drop connections that are taking too long (more than 10 seconds)
        for (size_t i = 0; i < ARR_LEN(g_sockets); i += 1) {
            if (!g_sockets[i].used) {
                continue;
            }
            if ((now - g_sockets[i].last_packet_received_at) > 10) {
                printf("closing connection with client that took more than 10 secs to send another packet.\n");
                login_on_disconnect(&g_state, g_sockets[i].client);
                unix_socket_close(&g_sockets[i]);
            }
        }

        for (int i = 0; i < ev_count; i += 1) {
            socket = (struct unix_socket *) events[i].data.ptr;

            // Make sure the socket hasn't been disconnected
            // due to 10 seconds timeout.
            if (!socket->used) {
                continue;
            }

            if (socket == server) {
                socket = find_free_socket();
                if (unix_socket_accept(socket, server)) {
                    printf("new connection accepted.\n");
                    socket->client = login_on_new_connection(&g_state);
                    if (socket->client) {
                        socket->client->playOK1 = rand();
                        socket->client->playOK2 = rand();
                        unix_socket_write(
                            socket,
                            socket->client->response.buf,
                            packet_size(&socket->client->response)
                        );
                        unix_socket_flush(socket);
                    } else {
                        printf("no client returned for connection. closing it.\n");
                        unix_socket_close(socket);
                    }
                }
                continue;
            }

            if ((events[i].events & EPOLLERR) == EPOLLERR) {
                printf("error in connection event, closing the connection.\n");
                login_on_disconnect(&g_state, socket->client);
                unix_socket_close(socket);
                continue;
            }

            if ((events[i].events & EPOLLOUT) == EPOLLOUT) {
                unix_socket_flush(socket);
            }

            if ((events[i].events & EPOLLIN) != EPOLLIN) {
                continue;
            }

            read = recv(
                socket->fd,
                socket->client->request.packet.buf + socket->client->request.received,
                sizeof(socket->client->request.packet.buf) - socket->client->request.received,
                0
            );

            if (read == 0) {
                printf("closing connection as requested by client.\n");
                login_on_disconnect(&g_state, socket->client);
                unix_socket_close(socket);
                continue;
            }

            if (read < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                }
                printf("failed to read packet from client, closing connection.\n");
                login_on_disconnect(&g_state, socket->client);
                unix_socket_close(socket);
                continue;
            }

            socket->client->request.received += (size_t) read;
            socket->last_packet_received_at = now;
            login_on_request(&g_state, socket->client);

            // If we have read all the packet then reset the
            // counter (stop appending data)
            if (!socket->client->request.is_partial) {
                socket->client->request.received = 0;
            }

            if (packet_size(&socket->client->response)) {
                unix_socket_write(
                    socket,
                    socket->client->response.buf,
                    packet_size(&socket->client->response)
                );
                unix_socket_flush(socket);
            }
        }
    }

    close(g_epoll_fd);

    return 1;
}

int main(int argc, char **argv)
{
    static struct unix_socket server = { 0 };
    u16 port = 2106;

    // Suppress unused warning.
    argc = argc;
    argv = argv;

    srand(time(0));

    if (!unix_socket_init(&server, port, ARR_LEN(g_sockets))) {
        printf("unable to initialize socket.\n");
        return 1;
    }

    if (!unix_socket_listen(&server)) {
        printf("unable to listen for requests.\n");
        return 1;
    }

    return 0;
}