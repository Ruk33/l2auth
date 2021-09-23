// Required for CLOCK_MONOTONIC to be defined.
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <netinet/in.h>

#include "../include/util.h"
#include "../include/platform.h"

#define MAX_SOCKETS (32)

#define MAX_TIMERS (1)

struct platform_socket {
        int fd;
        int is_server;
        int initialized;
};

struct platform_timer {
        int fd;
        int repeats;
        int stopped;
        int initialized;
};

static struct platform_socket sockets[MAX_SOCKETS] = { 0 };
static struct platform_timer timers[MAX_TIMERS]    = { 0 };

static struct platform_socket *find_free_socket(void)
{
        for (u32_t i = 0, max = UTIL_ARRAY_LEN(sockets); i < max; i += 1) {
                if (sockets[i].initialized) {
                        continue;
                }

                sockets[i] = (struct platform_socket){ 0 };

                return &sockets[i];
        }

        return 0;
}

/*
 * ---------------------
 * Socket implementation
 * ---------------------
 */

static void platform_socket_set_non_block(int fd)
{
        int flags = 0;

        flags = fcntl(fd, F_GETFL, 0);
        flags = flags < 0 ? 0 : flags;

        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

struct platform_socket *platform_socket_new(void)
{
        return find_free_socket();
}

struct platform_timer *platform_timer_new(void)
{
        return &timers[0];
}

void platform_socket_free(struct platform_socket *src)
{
        if (!src) {
                return;
        }
        *src = (struct platform_socket){ 0 };
}

void platform_timer_free(struct platform_timer *src)
{
        if (!src) {
                return;
        }
        *src = (struct platform_timer){ 0 };
}

int platform_socket_init(struct platform_socket *src, u16_t port, u32_t max)
{
        struct sockaddr_in address = { 0 };
        struct sockaddr *address_p = 0;

        int reusable_enable = 0;

        assert(src);
        assert(!src->initialized);
        assert(port);
        assert(max);

        src->fd          = socket(AF_INET, SOCK_STREAM, 0);
        src->initialized = 1;
        src->is_server   = 1;

        if (src->fd < 0) {
                goto abort;
        }

        // Make the socket reusable.
        reusable_enable = 1;

        if (setsockopt(
                    src->fd,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &reusable_enable,
                    sizeof(int)) < 0) {
                goto abort;
        }

        address.sin_family      = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port        = htons(port);

        address_p = (struct sockaddr *) &address;

        if (bind(src->fd, address_p, sizeof(address)) < 0) {
                goto abort;
        }

        if (listen(src->fd, (int) max) < 0) {
                goto abort;
        }

        platform_socket_set_non_block(src->fd);

        return 1;

abort:
        close(src->fd);
        *src = (struct platform_socket){ 0 };

        return 0;
}

int platform_socket_close(struct platform_socket *src)
{
        assert(src);
        assert(src->initialized);

        if (shutdown(src->fd, SHUT_RDWR) < 0) {
                return 0;
        }

        if (close(src->fd)) {
                return 0;
        }

        *src = (struct platform_socket){ 0 };

        return 1;
}

int platform_socket_accept(
        struct platform_socket *dest,
        struct platform_socket *src)
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

        platform_socket_set_non_block(dest->fd);

        return 1;
}

int platform_socket_listen(
        struct platform_socket *src,
        u32_t src_len,
        platform_socket_request_cb *cb)
{
        static struct epoll_event events[64] = { 0 };
        static byte_t buf[UTIL_MB(5)]        = { 0 };

        struct epoll_event event = { 0 };

        int epoll_fd = 0;
        int ev_count = 0;

        i32_t read = 0;

        if (!src || !src_len || !cb) {
                return 0;
        }

        epoll_fd = epoll_create1(0);

        if (epoll_fd < 0) {
                return 0;
        }

        while (1) {
                for (u32_t i = 0; i < src_len; i += 1) {
                        if (!src[i].initialized) {
                                continue;
                        }

                        // EPOLLIN: Ready to read.
                        // EPOLLOUT: Ready to write.
                        // EPOLLET: Only get events that ocurr after listening.
                        event.events  = EPOLLIN | EPOLLOUT | EPOLLET;
                        event.data.fd = src[i].fd;

                        // Duplicates won't be included.
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, src[i].fd, &event);
                }

                ev_count = epoll_wait(
                        epoll_fd, events, UTIL_ARRAY_LEN(events), -1);

                if (ev_count < 0) {
                        close(epoll_fd);
                        return 0;
                }

                for (u32_t i = 0; i < src_len; i += 1) {
                        if (!src[i].initialized) {
                                continue;
                        }

                        for (int n = 0; n < ev_count; n += 1) {
                                if (events[n].data.fd != src[i].fd) {
                                        continue;
                                }

                                if (src[i].is_server) {
                                        cb(&src[i],
                                           PLATFORM_SOCKET_NEW_CONNECTION,
                                           0,
                                           0);
                                        continue;
                                }

                                if ((events[n].events & EPOLLOUT) == EPOLLOUT) {
                                        cb(&src[i],
                                           PLATFORM_SOCKET_READY_TO_WRITE,
                                           0,
                                           0);
                                }

                                if ((events[n].events & EPOLLIN) != EPOLLIN) {
                                        continue;
                                }

                                read = recv(src[i].fd, buf, sizeof(buf), 0);

                                if (read < 0) {
                                        cb(&src[i],
                                           PLATFORM_SOCKET_FAILED_TO_READ,
                                           0,
                                           0);
                                        continue;
                                }

                                if (read == 0) {
                                        cb(&src[i],
                                           PLATFORM_SOCKET_DISCONNECTED,
                                           0,
                                           0);
                                        epoll_ctl(
                                                epoll_fd,
                                                EPOLL_CTL_ADD,
                                                src[i].fd,
                                                &event);
                                        src[i] = (struct platform_socket){ 0 };
                                        continue;
                                }

                                cb(&src[i],
                                   PLATFORM_SOCKET_NEW_REQUEST,
                                   buf,
                                   (u32_t) read);
                        }
                }
        }

        close(epoll_fd);

        return 1;
}

int platform_socket_send(
        struct platform_socket *dest,
        u32_t *sent,
        void *buf,
        u32_t n)
{
        i32_t tmp = 0;

        assert(dest);
        assert(sent);

        tmp = send(dest->fd, buf, n, 0);

        if (tmp < 0) {
                return 0;
        }

        *sent = (u32_t) tmp;

        return 1;
}

/*
 * -------------------------
 * End socket implementation
 * -------------------------
 */

/*
 * --------------------
 * Timer implementation
 * --------------------
 */

int platform_timer_init(
        struct platform_timer *src,
        u32_t ms_timeout,
        int repeats)
{
        struct itimerspec utmr = { 0 };

        if (!src) {
                return 0;
        }

        src->fd          = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
        src->initialized = 1;
        src->repeats     = repeats;
        src->stopped     = 0;

        if (src->fd < 0) {
                *src = (struct platform_timer){ 0 };
                return 0;
        }

        if (ms_timeout >= 1000) {
                utmr.it_value.tv_sec    = (long) (ms_timeout / 1000);
                utmr.it_interval.tv_sec = utmr.it_value.tv_sec;
        } else {
                utmr.it_value.tv_nsec    = (long) (ms_timeout * 1000000);
                utmr.it_interval.tv_nsec = utmr.it_value.tv_nsec;
        }

        if (timerfd_settime(src->fd, 0, &utmr, 0) < 0) {
                *src = (struct platform_timer){ 0 };
                return 0;
        }

        return 1;
}

int platform_timer_start(
        struct platform_timer *src,
        u32_t src_len,
        platform_timer_tick_cb *cb)
{
        static struct epoll_event events[64] = { 0 };

        struct epoll_event event = { 0 };

        int epoll_fd = 0;
        int ev_count = 0;

        struct itimerspec utmr = { 0 };

        if (!src || !src_len || !cb) {
                return 0;
        }

        epoll_fd = epoll_create1(0);

        if (epoll_fd < 0) {
                return 0;
        }

        while (1) {
                for (u32_t i = 0; i < src_len; i += 1) {
                        if (!src[i].initialized || src[i].stopped) {
                                continue;
                        }

                        event.events  = EPOLLIN | EPOLLET;
                        event.data.fd = src[i].fd;

                        // Duplicates won't be included.
                        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, src[i].fd, &event);
                }

                ev_count = epoll_wait(
                        epoll_fd, events, UTIL_ARRAY_LEN(events), -1);

                if (ev_count < 0) {
                        close(epoll_fd);
                        return 0;
                }

                for (u32_t i = 0; i < src_len; i += 1) {
                        if (src[i].stopped) {
                                event.data.fd = src[i].fd;
                                epoll_ctl(
                                        epoll_fd,
                                        EPOLL_CTL_DEL,
                                        src[i].fd,
                                        &event);
                                continue;
                        }

                        for (int n = 0; n < ev_count; n += 1) {
                                if (events[n].data.fd != src[i].fd) {
                                        continue;
                                }

                                cb(&src[i]);

                                if (src[i].repeats) {
                                        // Calling timerfd_gettime repeats the
                                        // timer...
                                        timerfd_gettime(src[i].fd, &utmr);
                                }
                        }
                }
        }

        close(epoll_fd);

        return 1;
}

void platform_timer_stop(struct platform_timer *src)
{
        if (!src) {
                return;
        }

        src->stopped = 1;
}

void platform_timer_resume(struct platform_timer *src)
{
        if (!src) {
                return;
        }

        src->stopped = 0;
}

/*
 * ------------------------
 * End timer implementation
 * ------------------------
 */

int platform_ip_to_u32(u32_t *dest, char *ip)
{
        struct sockaddr_in sa = { 0 };

        if (!dest || !ip) {
                return 0;
        }

        if (!inet_pton(AF_INET, ip, &(sa.sin_addr))) {
                return 0;
        }

        *dest = (u32_t) sa.sin_addr.s_addr;

        return 1;
}
