#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "memory.h"
#include "socket.h"

#define MAX_REQUESTS 10
#define MAX_REQUEST_SIZE 65536

#define EXPR_OR_RETURN_ERR(expr)        \
        do {                            \
                if ((expr) == -1) {     \
                        return -1;      \
                }                       \
        } while (0)

int socket_create(unsigned short port, size_t max_connections)
{
        int server = 0;
        struct sockaddr_in address = {0};

        assert(port);
        assert(max_connections);

        EXPR_OR_RETURN_ERR(server = socket(AF_INET, SOCK_STREAM, 0));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        EXPR_OR_RETURN_ERR(bind(server, (struct sockaddr *)&address, sizeof(address)));
        EXPR_OR_RETURN_ERR(listen(server, max_connections));

        return server;
}

int socket_accept(int fd)
{
        struct sockaddr_in address = {0};
        unsigned int addrlen = 0;

        return accept(fd, (struct sockaddr *) &address, &addrlen);
}

ssize_t socket_send(int fd, unsigned char *response, size_t response_size)
{
        ssize_t sent = 0;
        ssize_t tmp = 0;

        if (!fd || !response || !response_size) {
                return 0;
        }

        /*
         * TODO make sure we don't overflow
         */
        while (sent != (ssize_t) response_size) {
                tmp = send(fd, response + sent, response_size - sent, 0);

                if (tmp == -1) {
                        return -1;
                }

                sent += tmp;
        }

        return sent;
}

void socket_close(int fd)
{
        close(fd);
}

static void socket_set_non_blocking(int fd)
{
        int flags = 0;

// If they have O_NONBLOCK, use the Posix way to do it
#if defined(O_NONBLOCK)
        if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
                flags = 0;
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
        // Otherwise, use the old way of doing it
        flags = 1;
        ioctl(fd, FIOBIO, &flags);
#endif
}

static int socket_handle_conn(socket_conn_t *conn, int epoll_fd)
{
        struct epoll_event client_event = {0};

        client_event.events = EPOLLIN | EPOLLET;
        client_event.data.fd = conn->on_connect(conn->fd, conn->data);

        EXPR_OR_RETURN_ERR(client_event.data.fd);

        socket_set_non_blocking(client_event.data.fd);

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_event.data.fd, &client_event);

        return 0;
}

static ssize_t socket_receive(int fd, unsigned char *dest, size_t dest_size)
{
        ssize_t read = 0;
        ssize_t tmp = 0;

        assert(dest);

        /*
         * TODO make sure we don't overflow
         */
        while (1) {
                tmp = recv(fd, dest + read, dest_size - read, 0);

                if (read > 0 && tmp == -1) {
                        return read;
                }

                if (tmp == -1) {
                        return -1;
                }

                if (tmp == 0) {
                        break;
                }

                read += tmp;
        }

        return read;
}

static int socket_handle_events(socket_conn_t *conn, int epoll_fd)
{
        int events_count = 0;
        struct epoll_event events[MAX_REQUESTS] = {0};

        int fd = 0;
        int is_new_conn = 0;

        unsigned char *request = 0;
        ssize_t request_size = 0;

        assert(conn);

        request = memory_alloc(MAX_REQUEST_SIZE);

        EXPR_OR_RETURN_ERR(events_count = epoll_wait(epoll_fd, events, MAX_REQUESTS, -1));

        for (int i = 0; i < events_count; i++) {
                fd = events[i].data.fd;
                is_new_conn = fd == conn->fd;

                if (is_new_conn) {
                        socket_handle_conn(conn, epoll_fd);
                        continue;
                }

                memset(request, 0, MAX_REQUEST_SIZE);
                request_size = socket_receive(fd, request, MAX_REQUEST_SIZE);
                printf("Socket packet: %ld\n", request_size);

                if (request_size == 0) {
                        conn->on_disconnect(fd, conn->data);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                        continue;
                }

                conn->on_request(fd, conn->data, request, request_size);
        }

        memory_free(request);

        return 0;
}

int socket_handle_requests(socket_conn_t *conn)
{
        int epoll_fd = 0;
        struct epoll_event event = {0};

        assert(conn);

        socket_set_non_blocking(conn->fd);

        EXPR_OR_RETURN_ERR(epoll_fd = epoll_create1(0));

        event.events = EPOLLIN;
        event.data.fd = conn->fd;

        EXPR_OR_RETURN_ERR(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn->fd, &event));

        while (1) {
                EXPR_OR_RETURN_ERR(socket_handle_events(conn, epoll_fd));
        }

        return 0;
}
