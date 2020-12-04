#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket.h"

#define MAX_REQUESTS 10
#define MAX_REQUEST_SIZE 65536

#define EXPR_OR_RETURN_ERR(expr)        \
        do                              \
        {                               \
                if ((expr) == -1)       \
                {                       \
                        return -1;      \
                }                       \
        } while (0)

int socket_create(unsigned short port, size_t max_connections)
{
        assert(port);
        assert(max_connections);

        int server = 0;
        struct sockaddr_in address = {0};

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
        assert(fd > 0);

        struct sockaddr_in address = {0};
        unsigned int addrlen = 0;

        return accept(fd, (struct sockaddr *)&address, &addrlen);
}

size_t socket_send(int fd, unsigned char *response, size_t response_size)
{
        assert(fd > 0);
        assert(response);
        assert(response_size);
        return send(fd, response, response_size, 0);
}

void socket_close(int fd)
{
	assert(fd > 0);
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

static int socket_handle_conn(int fd, void *data, int epoll_fd, socket_on_conn_cb conn)
{
        struct epoll_event client_event = {0};

        client_event.events = EPOLLIN | EPOLLET;
        client_event.data.fd = conn(fd, data);

        EXPR_OR_RETURN_ERR(client_event.data.fd);

        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_event.data.fd, &client_event);

        return 0;
}

static int socket_handle_events(int fd, void *data, int epoll_fd, socket_on_conn_cb conn, socket_on_request_cb req, socket_on_disconnect_cb dis)
{
        assert(fd > 0);
        assert(epoll_fd > 0);

        int events_count = 0;
        struct epoll_event events[MAX_REQUESTS] = {0};

        int is_new_conn = 0;

        unsigned char *request = malloc(MAX_REQUEST_SIZE);
        size_t request_size = 0;

        EXPR_OR_RETURN_ERR(events_count = epoll_wait(epoll_fd, events, MAX_REQUESTS, -1));

        for (int i = 0; i < events_count; i++)
        {
                is_new_conn = events[i].data.fd == fd;

                if (is_new_conn)
                {
                        socket_handle_conn(fd, data, epoll_fd, conn);
                        continue;
                }

                memset(request, 0, MAX_REQUEST_SIZE);
                request_size = recv(events[i].data.fd, request, MAX_REQUEST_SIZE, 0);

                if (request_size == 0)
                {
                        dis(events[i].data.fd, data);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                        continue;
                }

                req(events[i].data.fd, data, request, request_size);
        }

        free(request);

        return 0;
}

int socket_handle_requests(int fd, void *data, socket_on_conn_cb conn, socket_on_request_cb req, socket_on_disconnect_cb dis)
{
        assert(fd > 0);
        assert(conn);
        assert(req);
        assert(dis);

        int epoll_fd = 0;
        struct epoll_event event = {0};

        socket_set_non_blocking(fd);

        EXPR_OR_RETURN_ERR(epoll_fd = epoll_create1(0));

        event.events = EPOLLIN;
        event.data.fd = fd;

        EXPR_OR_RETURN_ERR(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event));

        while (1)
        {
                EXPR_OR_RETURN_ERR(socket_handle_events(fd, data, epoll_fd, conn, req, dis));
        }

        return 0;
}
