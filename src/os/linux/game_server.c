#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "../../include/util.h"
#include "../../include/packet.h"
#include "../../server/game/include/l2_string.h"
#include "../../server/game/include/client.h"
#include "../../server/game/include/state.h"
#include "../../server/game/include/game.h"

#include "../../util.c"
#include "../../packet.c"
#include "../../server/game/packet_read.c"
#include "../../server/game/l2_string.c"
#include "../../server/game/state.c"
#include "../../server/game/client.c"
#include "../../server/game/client_packet.c"
#include "../../server/game/server_packet.c"
#include "../../server/game/game.c"

#ifndef MAX_SOCKETS
#define MAX_SOCKETS 512
#endif

struct unix_socket {
    int fd;
    byte buf_write[KB(4)];
    size_t sent;
    size_t write_size;
    struct client *client;
};

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

    if (!dest) {
        printf("unable to accept new client. too many clients?\n");
        return 0;
    }

    address_p = (struct sockaddr *) &address;
    dest->fd = accept(src->fd, address_p, &addrlen);

    if (dest->fd < 0) {
        *dest = (struct unix_socket){ 0 };
        printf("failed to accept new client.\n");
        return 0;
    }

    unix_socket_set_non_block(dest->fd);
    return 1;
}

static void unix_socket_close(struct unix_socket *socket)
{
    shutdown(socket->fd, SHUT_RDWR);
    close(socket->fd);
    *socket = (struct unix_socket){ 0 };
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
    cpy_bytes(socket->buf_write + socket->write_size, buf, n);
    socket->write_size += n;
}

static void sighandler(int signum)
{
    printf("caught signal %d, coming out...\n", signum);
}

static void fork_and_listen(struct unix_socket *server)
{
    static struct state state = { 0 };
    struct unix_socket client = { 0 };

    pid_t pid = 0;

    struct epoll_event events[8] = { 0 };
    struct epoll_event event = { 0 };
    int epoll_fd = 0;
    int ev_count = 0;

    ssize_t read = 0;

    if (!unix_socket_accept(&client, server)) {
        printf("unable to accept new client.\n");
        return;
    }

    pid = fork();
    switch (pid) {
    case -1: // Something went wrong
        printf("unable to fork. closing the connection.\n");
        unix_socket_close(&client);
        break;
    case 0: // Child
        break;
    default: // Parent
        printf("handling new connection in process %d.\n", pid);
        return;
    }

    signal(SIGTERM, sighandler);

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        printf("unable to create epoll from client. closing connection.\n");
        unix_socket_close(&client);
        exit(EXIT_FAILURE);
    }

    event.events = EPOLLIN | EPOLLOUT | EPOLLET;
    event.data.ptr = &client;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client.fd, &event) == -1) {
        printf("failed to epoll add. closing the connection.\n");
        unix_socket_close(&client);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    client.client = game_on_new_connection(&state);
    if (!client.client) {
        printf("client couldn't initialize. closing the connection.\n");
        unix_socket_close(&client);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
    unix_socket_write(
        &client,
        client.client->response.buf,
        packet_size(&client.client->response)
    );
    unix_socket_flush(&client);

    while (1) {
        ev_count = epoll_wait(epoll_fd, events, ARR_LEN(events), -1);

        if (ev_count < 0) {
            printf("epoll_wait failed, closing the client. closing connection.\n");
            game_on_disconnect(&state, client.client);
            unix_socket_close(&client);
            close(epoll_fd);
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < ev_count; i += 1) {
            if ((events[i].events & EPOLLIN) == EPOLLIN) {
                unix_socket_flush(&client);
            }

            if ((events[i].events & EPOLLOUT) != EPOLLOUT) {
                continue;
            }
            
            read = recv(
                client.fd,
                client.client->request.buf + client.client->received,
                sizeof(client.client->request.buf) - client.client->received,
                0
            );

            if (read == 0) {
                printf("closing connection as requested by client.\n");
                game_on_disconnect(&state, client.client);
                unix_socket_close(&client);
                close(epoll_fd);
                exit(EXIT_SUCCESS);
            }

            if (read < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                }
                printf("failed to read packet from client, closing connection.\n");
                game_on_disconnect(&state, client.client);
                unix_socket_close(&client);
                close(epoll_fd);
                exit(EXIT_FAILURE);
            }

            client.client->received += (size_t) read;
            game_on_request(&state, client.client);

            if (packet_size(&client.client->response)) {
                unix_socket_write(
                    &client,
                    client.client->response.buf,
                    packet_size(&client.client->response)
                );
                unix_socket_flush(&client);
            }
        }
    }

    unix_socket_close(&client);
    close(epoll_fd);
    exit(EXIT_SUCCESS);
}

static int unix_socket_listen(struct unix_socket *server)
{
    static struct epoll_event events[MAX_SOCKETS] = { 0 };
    struct epoll_event event = { 0 };
    int epoll_fd = 0;
    int ev_count = 0;

    assert(server);

    epoll_fd = epoll_create1(0);

    if (epoll_fd < 0) {
        printf("unable to create epoll. server can't start.\n");
        return 0;
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.ptr = server;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->fd, &event) == -1) {
        printf("unable to add server socket to epoll. server can't start.\n");
        return 0;
    }
    printf("listening for new connections.\n");

    while (1) {
        ev_count = epoll_wait(epoll_fd, events, ARR_LEN(events), -1);

        if (ev_count < 0) {
            printf("epoll_wait failed, closing the server.\n");
            break;
        }

        for (int i = 0; i < ev_count; i += 1) {
            if ((events[i].events & EPOLLIN) == EPOLLIN) {
                fork_and_listen(server);
            }
        }
    }

    unix_socket_close(server);
    close(epoll_fd);

    return 1;
}

int main(int argc, char **argv)
{
    static struct unix_socket server = { 0 };
    u16 port = 7777;

    // Suppress unused warning.
    argc = argc;
    argv = argv;

    srand(time(0));

    if (!unix_socket_init(&server, port, MAX_SOCKETS)) {
        printf("unable to initialize socket.\n");
        return 1;
    }

    if (!unix_socket_listen(&server)) {
        printf("unable to listen for requests.\n");
        return 1;
    }

    return 0;
}