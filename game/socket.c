#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "memory.h"
#include "socket.h"

#define MAX_REQUESTS 100
#define MAX_REQUEST_SIZE 65536

#define EXPR_OR_RETURN_ERR(expr)    \
        do {                        \
                if ((expr) == -1) { \
                        return -1;  \
                }                   \
        } while (0)

struct Client {
        int active;
        int fd;
        unsigned char *read_buffer;
        unsigned char *write_buffer;
        size_t written_bytes;
        size_t to_be_written;
};

static struct Client *clients = NULL;
static struct epoll_event *events = NULL;

static void init_client(struct Client *client, int fd)
{
        assert(client);

        client->fd = fd;
        client->active = 1;
        client->read_buffer = memory_alloc(MAX_REQUEST_SIZE);
        client->write_buffer = memory_alloc(MAX_REQUEST_SIZE);
        client->written_bytes = 0;
        client->to_be_written = 0;
}

static struct Client *client_by_fd(int fd)
{
        for (int i = 0; i < MAX_REQUESTS; i++) {
                if (clients[i].fd == fd) {
                        return &clients[i];
                }
        }

        return NULL;
}

int socket_create(unsigned short port, size_t max_connections)
{
        int server = 0;
        struct sockaddr_in address = { 0 };

        assert(port);
        assert(max_connections);

        EXPR_OR_RETURN_ERR(server = socket(AF_INET, SOCK_STREAM, 0));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        EXPR_OR_RETURN_ERR(
                bind(server, (struct sockaddr *) &address, sizeof(address)));
        EXPR_OR_RETURN_ERR(listen(server, max_connections));

        return server;
}

int socket_accept(int fd)
{
        struct sockaddr_in address = { 0 };
        unsigned int addrlen = 0;
        return accept(fd, (struct sockaddr *) &address, &addrlen);
}

ssize_t socket_send(int fd, unsigned char *response, size_t response_size)
{
        struct Client *client = NULL;
        ssize_t sent_bytes = 0;
        int can_not_write = 0;

        if (!response || !response_size) {
                return 0;
        }

        client = client_by_fd(fd);

        if (!client) {
                return -1;
        }

        memcpy(client->write_buffer + client->to_be_written, response,
               response_size);
        client->to_be_written += response_size;

        sent_bytes = send(fd, client->write_buffer + client->written_bytes,
                          client->to_be_written - client->written_bytes, 0);
        can_not_write = sent_bytes < 0 &&
                        (errno == EWOULDBLOCK || errno == EAGAIN);

        if (can_not_write) {
        } else if (sent_bytes > 0) {
                client->written_bytes += sent_bytes;
                if (client->written_bytes == client->to_be_written) {
                        client->written_bytes = 0;
                        client->to_be_written = 0;
                }
        }

        return sent_bytes;
}

void socket_close(int fd)
{
        struct Client *client = NULL;

        close(fd);

        client = client_by_fd(fd);

        if (!client) {
                return;
        }

        client->active = 0;
        memory_free(client->read_buffer);
        memory_free(client->write_buffer);
}

static void socket_set_non_blocking(int fd)
{
        int flags = 0;
        if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
                flags = 0;
        }
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static int socket_handle_conn(socket_conn_t *conn, int epoll_fd)
{
        size_t free_client_index = 0;
        struct Client *client = NULL;
        struct epoll_event client_event = { 0 };
        int client_fd = 0;

        client_fd = conn->on_connect(conn->fd, conn->data);
        EXPR_OR_RETURN_ERR(client_fd);
        socket_set_non_blocking(client_fd);

        /*
         * Find a free client to be used.
         */
        do {
                client = &clients[free_client_index];
                free_client_index += 1;
        } while (free_client_index < MAX_REQUESTS && client->active);

        init_client(client, client_fd);

        client_event.events = EPOLLIN;
        client_event.data.fd = client_fd;

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_event.data.fd, &client_event);

        return 0;
}

static int socket_handle_events(socket_conn_t *conn, int epoll_fd)
{
        int events_count = 0;

        int fd = 0;
        int is_new_conn = 0;
        struct Client *client = NULL;
        ssize_t tmp = 0;

        int can_not_read = 0;
        int can_not_write = 0;

        assert(conn);

        EXPR_OR_RETURN_ERR(
                events_count = epoll_wait(epoll_fd, events, MAX_REQUESTS, -1));

        for (int i = 0; i < events_count; i++) {
                fd = events[i].data.fd;
                is_new_conn = fd == conn->fd;

                if (is_new_conn) {
                        socket_handle_conn(conn, epoll_fd);
                        continue;
                }

                client = client_by_fd(fd);

                if (!client) {
                        continue;
                }

                /*
                 * Read
                 */
                if (events[i].events & EPOLLIN) {
                        memset(client->read_buffer, 0, MAX_REQUEST_SIZE);

                        tmp = recv(fd, client->read_buffer, MAX_REQUEST_SIZE,
                                   0);
                        can_not_read = tmp < 0 && (errno == EAGAIN ||
                                                   errno == EWOULDBLOCK);

                        if (can_not_read) {
                                // Not sure if this could ever happen?
                        } else if (tmp > 0) {
                                conn->on_request(fd, conn->data,
                                                 client->read_buffer, tmp);
                        } else {
                                conn->on_disconnect(fd, conn->data);
                                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                        }
                }

                /*
                 * Write
                 */
                if (events[i].events & EPOLLOUT) {
                        tmp = send(fd,
                                   client->write_buffer + client->written_bytes,
                                   client->to_be_written -
                                           client->written_bytes,
                                   0);
                        can_not_write = tmp < 0 && (errno == EAGAIN ||
                                                    errno == EWOULDBLOCK);

                        if (can_not_write) {
                                // Not sure if this could ever happen?
                        } else if (tmp > 0) {
                                client->written_bytes += tmp;
                                if (client->written_bytes ==
                                    client->to_be_written) {
                                        client->written_bytes = 0;
                                        client->to_be_written = 0;
                                }
                        }
                }
        }

        return 0;
}

int socket_handle_requests(socket_conn_t *conn)
{
        int epollfd = 0;
        struct epoll_event event = { 0 };

        assert(conn);

        socket_set_non_blocking(conn->fd);
        EXPR_OR_RETURN_ERR(epollfd = epoll_create1(0));

        event.events = EPOLLIN;
        event.data.fd = conn->fd;

        EXPR_OR_RETURN_ERR(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn->fd, &event));

        events = memory_alloc(sizeof(*events) * MAX_REQUESTS);
        clients = memory_alloc(sizeof(*clients) * MAX_REQUESTS);

        while (1) {
                EXPR_OR_RETURN_ERR(socket_handle_events(conn, epollfd));
        }

        return 0;
}
