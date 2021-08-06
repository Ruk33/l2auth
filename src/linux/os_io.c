// Required for CLOCK_MONOTONIC to be defined.
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <assert.h>      // assert
#include <unistd.h>      // close
#include <fcntl.h>       // fcbtk, O_NONBLOCK, etc
#include <sys/socket.h>  // socket
#include <sys/epoll.h>   // epoll
#include <sys/timerfd.h> // timerfd
#include <netinet/in.h>  // sockaddr, htons, etc.
#include "../include/util.h"
#include "../include/recycle_id.h"
#include "../include/os_io.h"

#define MAX_CONN 1024
#define READ_BUF_SIZE 524288

enum io_type {
        SERVER_SOCKET,
        CLIENT_SOCKET,
        TIMER,
};

struct os_io {
        size_t id;
        int fd;
        enum io_type type;
};

static size_t instances[MAX_CONN] = { 0 };

static struct os_io ios[MAX_CONN] = { 0 };

static size_t io_count = 0;

static struct epoll_event events[MAX_CONN] = { 0 };

static struct os_io *io_find(int fd)
{
        for (size_t i = 0; i < io_count; i += 1) {
                if (ios[i].fd == fd) {
                        return &ios[i];
                }
        }

        return 0;
}

static struct os_io *io_add(int fd, enum io_type type)
{
        size_t instance = 0;

        assert(io_count < arr_size(ios));

        io_count += recycle_id_get(&instance, instances);

        ios[instance].id   = instance;
        ios[instance].fd   = fd;
        ios[instance].type = type;

        return &ios[instance];
}

static void io_remove(struct os_io *io)
{
        assert(io);
        recycle_id(instances, io->id);
        *io = (struct os_io){ 0 };
}

static int io_add_to_epoll(int epoll_fd, int fd)
{
        struct epoll_event event = { 0 };

        event.events  = EPOLLIN | EPOLLOUT | EPOLLET;
        event.data.fd = fd;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
                return 0;
        }

        return 1;
}

static void io_remove_epoll(int epoll_fd, int fd)
{
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, 0);
}

static void io_socket_set_non_blocking(int fd)
{
        int flags = 0;

        flags = fcntl(fd, F_GETFL, 0);

        if (flags == -1) {
                flags = 0;
        }

        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

static struct os_io *io_accept_connection(int epoll_fd, int server_fd)
{
        struct sockaddr_in address = { 0 };
        struct sockaddr *address_p = 0;
        socklen_t addrlen          = 0;

        int new_socket = 0;

        address_p  = (struct sockaddr *) &address;
        new_socket = accept(server_fd, address_p, &addrlen);

        if (new_socket == -1) {
                return 0;
        }

        io_socket_set_non_blocking(new_socket);

        if (!io_add_to_epoll(epoll_fd, new_socket)) {
                return 0;
        }

        return io_add(new_socket, CLIENT_SOCKET);
}

struct os_io *os_io_socket_create(u16_t port, size_t max_conn)
{
        int new_socket = 0;

        struct sockaddr_in address = { 0 };
        struct sockaddr *address_p = 0;

        int reusable_enable = 1;

        assert(max_conn < MAX_CONN);

        new_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (new_socket == -1) {
                return 0;
        }

        if (setsockopt(
                    new_socket,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &reusable_enable,
                    sizeof(int)) < 0) {
                return 0;
        }

        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port        = htons(port);

        address_p = (struct sockaddr *) &address;

        if (bind(new_socket, address_p, sizeof(address)) == -1) {
                return 0;
        }

        if (listen(new_socket, (int) max_conn) == -1) {
                return 0;
        }

        io_socket_set_non_blocking(new_socket);

        return io_add(new_socket, SERVER_SOCKET);
}

int os_io_close(struct os_io *io)
{
        assert(io);
        io_remove(io);
        return close(io->fd);
}

ssize_t os_io_write(struct os_io *io, void *buf, size_t n)
{
        assert(io);
        return send(io->fd, buf, n, 0);
}

struct os_io *os_io_timer(double timeout)
{
        int timer = 0;

        struct itimerspec utmr = { 0 };

        timer = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

        if (timer == -1) {
                return 0;
        }

        utmr.it_value.tv_nsec    = timeout * 1000000000;
        utmr.it_interval.tv_nsec = timeout * 1000000000;

        timerfd_settime(timer, 0, &utmr, 0);

        return io_add(timer, TIMER);
}

int os_io_listen(os_io_cb *cb)
{
        static byte_t buf[READ_BUF_SIZE] = { 0 };

        int epoll_fd = 0;
        int ev_count = 0;

        ssize_t read = 0;

        struct itimerspec utmr = { 0 };

        struct os_io *io = 0;

        assert(cb);

        epoll_fd = epoll_create1(0);

        if (epoll_fd == -1) {
                return 0;
        }

        for (size_t i = 0; i < io_count; i += 1) {
                io_add_to_epoll(epoll_fd, ios[i].fd);
        }

        while (1) {
                ev_count = epoll_wait(epoll_fd, events, sizeof(events), -1);

                if (ev_count == -1) {
                        return 0;
                }

                for (int i = 0; i < ev_count; i += 1) {
                        io = io_find(events[i].data.fd);

                        if (!io) {
                                continue;
                        }

                        switch (io->type) {
                        case SERVER_SOCKET:
                                io = io_accept_connection(epoll_fd, io->fd);
                                cb(io, OS_IO_SOCKET_CONNECTION, 0, 0);
                                break;
                        case CLIENT_SOCKET:
                                read = recv(io->fd, buf, sizeof(buf), 0);

                                switch (read) {
                                case -1:
                                        cb(io, OS_IO_SOCKET_READ_ERROR, 0, 0);
                                        break;
                                case 0:
                                        cb(io, OS_IO_SOCKET_DISCONNECTED, 0, 0);
                                        io_remove_epoll(epoll_fd, io->fd);
                                        io_remove(io);
                                        break;
                                default:
                                        cb(io, OS_IO_SOCKET_REQUEST, buf, read);
                                        break;
                                }
                                break;
                        case TIMER:
                                // Calling timerfd_gettime repeats the timer...
                                timerfd_gettime(io->fd, &utmr);
                                cb(io, OS_IO_TIMER_TICK, 0, 0);
                                break;
                        default:
                                break;
                        }
                }
        }

        return 1;
}
