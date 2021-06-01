#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "../include/config.h"
#include "../include/socket.h"

#define READ_BUF_SIZE 65536
#define WRITE_BUF_SIZE 65536
#define MAX_REQUESTS 256

static int    sockets[MAX_CLIENTS] = { 0 };
static size_t socket_count         = 0;

static struct epoll_event events[MAX_REQUESTS] = { 0 };

static byte_t read_buf[READ_BUF_SIZE] = { 0 };

static byte_t write_buf[MAX_CLIENTS][WRITE_BUF_SIZE] = { 0 };
static size_t to_be_sent[MAX_CLIENTS]                = { 0 };

static int socket_to_fd(socket_t *src)
{
        return *((int *) src);
}

static size_t socket_id(socket_t *socket)
{
        for (size_t i = 0; i < socket_count; i += 1) {
                if (&sockets[i] == socket) {
                        return i;
                }
        }

        return 0;
}

static socket_t *fd_to_socket(int fd)
{
        for (size_t i = 0; i < socket_count; i += 1) {
                if (sockets[i] == fd) {
                        return &sockets[i];
                }
        }

        return 0;
}

static void socket_set_non_blocking(int fd)
{
        int flags = 0;

        if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
                flags = 0;
        }

        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

socket_t *socket_create(u16_t port)
{
        int new_socket = 0;

        struct sockaddr_in address   = { 0 };
        struct sockaddr *  address_p = 0;

        new_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (new_socket == -1) {
                return 0;
        }

        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port        = htons(port);

        address_p = (struct sockaddr *) &address;

        if (bind(new_socket, address_p, sizeof(address)) == -1) {
                return 0;
        }

        if (listen(new_socket, MAX_CLIENTS) == -1) {
                return 0;
        }

        socket_set_non_blocking(new_socket);

        sockets[socket_count] = new_socket;
        socket_count += 1;

        return &sockets[socket_count - 1];
}

static socket_t *socket_accept(socket_t *socket)
{
        struct sockaddr_in address   = { 0 };
        struct sockaddr *  address_p = 0;
        socklen_t          addrlen   = 0;

        int new_socket = 0;

        address_p  = (struct sockaddr *) &address;
        new_socket = accept(socket_to_fd(socket), address_p, &addrlen);

        if (new_socket == -1) {
                return 0;
        }

        socket_set_non_blocking(new_socket);

        sockets[socket_count] = new_socket;
        socket_count += 1;

        return &sockets[socket_count - 1];
}

ssize_t socket_send(socket_t *socket, byte_t *src, size_t n)
{
        int     id   = 0;
        ssize_t sent = 0;

        id = socket_id(socket);

        bytes_cpy(write_buf[id] + to_be_sent[id], src, n);
        to_be_sent[id] += n;

        sent = send(socket_to_fd(socket), write_buf[id], to_be_sent[id], 0);
        to_be_sent[id] -= sent;

        for (size_t i = 0; i < to_be_sent[id]; i += 1) {
                write_buf[id][i] = write_buf[id][sent + i];
        }

        return sent;
}

int socket_close(socket_t *socket)
{
        // Todo: Do this properly.
        socket_count -= 1;
        return close(socket_to_fd(socket));
}

int socket_handle_requests(socket_t *socket, socket_ev_cb cb)
{
        int fd       = 0;
        int epoll_fd = 0;

        struct epoll_event event        = { 0 };
        struct epoll_event client_event = { 0 };

        socket_t *client_socket = 0;

        int event_count = 0;

        ssize_t read = 0;

        epoll_fd = epoll_create1(0);

        if (epoll_fd == -1) {
                return 0;
        }

        fd = socket_to_fd(socket);

        event.events  = EPOLLIN;
        event.data.fd = fd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
                return 0;
        }

        while (1) {
                event_count = epoll_wait(epoll_fd, events, MAX_REQUESTS, -1);

                if (event_count == -1) {
                        return 0;
                }

                for (int i = 0; i < event_count; i += 1) {
                        fd = events[i].data.fd;

                        // New connection.
                        if (fd == event.data.fd) {
                                client_socket = socket_accept(socket);
                                cb(client_socket, CONN, 0, 0);

                                client_event.events = EPOLLIN;
                                client_event.data.fd =
                                        socket_to_fd(client_socket);

                                epoll_ctl(
                                        epoll_fd,
                                        EPOLL_CTL_ADD,
                                        client_event.data.fd,
                                        &client_event);
                                continue;
                        }

                        // Reading.
                        if (events[i].events & EPOLLIN) {
                                read = recv(fd, read_buf, READ_BUF_SIZE, 0);

                                switch (read) {
                                case -1:
                                        // Todo: Something went wrong.
                                        break;
                                case 0:
                                        // Closed connection.
                                        cb(fd_to_socket(fd), CLOSED, 0, 0);
                                        epoll_ctl(
                                                epoll_fd,
                                                EPOLL_CTL_DEL,
                                                fd,
                                                NULL);
                                        break;
                                default:
                                        // New request.
                                        cb(fd_to_socket(fd),
                                           REQ,
                                           read_buf,
                                           read);
                                        break;
                                }
                        }

                        // Write.
                        if (events[i].events & EPOLLOUT) {
                                socket_send(fd_to_socket(fd), 0, 0);
                        }
                }
        }

        return 1;
}
