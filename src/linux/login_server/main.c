#include <assert.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../../include/config.h"
#include "../../include/util.h"
#include "../../include/platform.h"
#include "../../include/ls_types.h"

#include "../../ls_lib.c"

struct platform_socket {
    int fd;
    int is_server;
    int initialized;
};

enum platform_fd_request_type
{
    PLATFORM_SOCKET_NEW_CONNECTION,
    PLATFORM_SOCKET_NEW_REQUEST,
    PLATFORM_SOCKET_FAILED_TO_READ,
    PLATFORM_SOCKET_DISCONNECTED,
};

static struct platform_socket g_sockets[MAX_CLIENTS] = { 0 };

static int g_epoll_fd = 0;

static void on_request(struct platform_socket *socket,
                       enum platform_fd_request_type type,
                       void *buf,
                       size_t n);

// Returns 1 on success.
static int fd_to_poll(int fd, void *ptr, int events)
{
    struct epoll_event event = { 0 };

    event.events   = events;
    event.data.ptr = ptr;

    return epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, fd, &event) != 0;
}

static struct platform_socket *get_free_socket(void)
{
    for (size_t i = 0; i < MAX_CLIENTS; i += 1) {
        if (!g_sockets[i].initialized) {
            return &g_sockets[i];
        }
    }
    return 0;
}

static void socket_non_block(int fd)
{
    int flags = 0;

    flags = fcntl(fd, F_GETFL, 0);
    flags = flags < 0 ? 0 : flags;

    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int socket_accept(struct platform_socket *dest, struct platform_socket *src)
{
    struct sockaddr_in address = { 0 };
    struct sockaddr *address_p = 0;

    socklen_t addrlen = 0;

    assert(dest);
    assert(src);
    assert(src->initialized);
    assert(src->is_server);

    address_p = (struct sockaddr *) &address;

    dest->fd          = accept(src->fd, address_p, &addrlen);
    dest->initialized = 1;
    dest->is_server   = 0;

    if (dest->fd < 0) {
        *dest = (struct platform_socket){ 0 };
        return 0;
    }

    socket_non_block(dest->fd);
    fd_to_poll(dest->fd, dest, EPOLLIN | EPOLLET);

    return 1;
}

static int block_and_listen_for_requests(void)
{
    // (franco.montenegro) I don't know what would be a
    // good size for this one.
    static struct epoll_event events[32] = { 0 };
    // (franco.montenegro) Should we put this memory
    // in the heap?
    static byte_t buf[macro_util_kb(8)] = { 0 };

    struct sockaddr_in server_address    = { 0 };
    struct sockaddr *server_address_p    = 0;
    struct platform_socket server_socket = { 0 };

    struct platform_socket *platform_socket = 0;

    int reusable_enable = 0;

    int ev_count = 0;

    ssize_t read = 0;

    server_socket.fd          = socket(AF_INET, SOCK_STREAM, 0);
    server_socket.initialized = 1;
    server_socket.is_server   = 1;

    if (server_socket.fd < 0) {
        printf("failed to create server socket.\n");
        return 0;
    }

    // Make the socket reusable.
    reusable_enable = 1;

    if (setsockopt(server_socket.fd,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &reusable_enable,
                   sizeof(int)) < 0) {
        close(server_socket.fd);
        printf("failed to set server socket as reusable.\n");
        return 0;
    }

    server_address.sin_family      = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port        = htons(2106);

    server_address_p = (struct sockaddr *) &server_address;

    if (bind(server_socket.fd, server_address_p, sizeof(server_address)) < 0) {
        close(server_socket.fd);
        printf("failed to bind server socket.\n");
        return 0;
    }

    if (listen(server_socket.fd, MAX_CLIENTS) < 0) {
        close(server_socket.fd);
        printf("failed to listen in server socket.\n");
        return 0;
    }

    socket_non_block(server_socket.fd);
    fd_to_poll(server_socket.fd, &server_socket, EPOLLIN);

    while (1) {
        ev_count =
            epoll_wait(g_epoll_fd, events, macro_util_arr_len(events), -1);

        if (ev_count < 0) {
            printf("failed to get epoll events.\n");
            break;
        }

        for (int i = 0; i < ev_count; i += 1) {
            // We only care for EPOLLIN (read) events.
            // Skip all the rest.
            if ((events[i].events & EPOLLIN) != EPOLLIN) {
                continue;
            }

            platform_socket = events[i].data.ptr;

            if (platform_socket->is_server) {
                on_request(platform_socket,
                           PLATFORM_SOCKET_NEW_CONNECTION,
                           0,
                           0);
                continue;
            }

            read = recv(platform_socket->fd, buf, sizeof(buf), 0);

            if (read < 0) {
                on_request(platform_socket,
                           PLATFORM_SOCKET_FAILED_TO_READ,
                           0,
                           0);
                continue;
            }

            if (read == 0) {
                on_request(platform_socket, PLATFORM_SOCKET_DISCONNECTED, 0, 0);
                epoll_ctl(g_epoll_fd, EPOLL_CTL_DEL, platform_socket->fd, 0);
                *platform_socket = (struct platform_socket){ 0 };
                continue;
            }

            on_request(platform_socket,
                       PLATFORM_SOCKET_NEW_REQUEST,
                       buf,
                       (size_t) read);
        }
    }

    close(server_socket.fd);

    return 1;
}

static void send_response(struct platform_socket *socket, void *buf, size_t n)
{
    ssize_t sent = 0;

    if (!n) {
        return;
    }

    if (!socket || !buf) {
        printf("internal send response, no io? ignoring.\n");
        return;
    }

    sent = send(socket->fd, buf, n, 0);

    if (sent < (ssize_t) n) {
        printf(
            "WARNING - send_response wanted to send %ld but %ld were sent.\n",
            n,
            sent);
    }
}

static void disconnect(struct platform_socket *socket)
{
    if (!socket) {
        return;
    }

    // ls_lib_disconnect(socket);

    shutdown(socket->fd, SHUT_RDWR);
    close(socket->fd);

    *socket = (struct platform_socket){ 0 };
}

static void on_request(struct platform_socket *socket,
                       enum platform_fd_request_type type,
                       void *buf,
                       size_t n)
{
    struct platform_socket *client_socket = 0;

    switch (type) {
    case PLATFORM_SOCKET_NEW_CONNECTION:
        client_socket = get_free_socket();

        if (!socket_accept(client_socket, socket)) {
            printf("login server unable to accept new connection.\n");
            return;
        }

        ls_lib_new_conn(client_socket);
        break;
    case PLATFORM_SOCKET_NEW_REQUEST:
        ls_lib_new_req(socket, buf, n);
        break;
    case PLATFORM_SOCKET_FAILED_TO_READ:
        printf("login server failed to read packet.\n");
        break;
    case PLATFORM_SOCKET_DISCONNECTED:
        printf("login server client disconnected.\n");
        break;
    default:
        break;
    }
}

static u32_t ip_to_u32(char *ip)
{
    struct sockaddr_in sa = { 0 };

    if (!ip) {
        return 0;
    }

    if (!inet_pton(AF_INET, ip, &(sa.sin_addr))) {
        return 0;
    }

    return (u32_t) sa.sin_addr.s_addr;
}

int main(/* int argc, char **argv */)
{
    static struct ls_state ls = { 0 };

    g_epoll_fd = epoll_create1(0);

    if (g_epoll_fd < 0) {
        printf("failed to create epoll.\n");
        return 1;
    }

    ls.send_response  = send_response;
    ls.disconnect     = disconnect;
    ls.text_ip_to_u32 = ip_to_u32;

    ls_lib_load(&ls);

    printf("login server started.\n");

    if (!block_and_listen_for_requests()) {
        printf("login server socket unable to listen for new connections.\n");
        return 1;
    }

    close(g_epoll_fd);

    return 0;
}
