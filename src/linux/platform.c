// Required for CLOCK_MONOTONIC to be defined.
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <netinet/in.h>

#include "../include/util.h"
#include "../include/platform.h"

#define MAX_SOCKETS (32)

#define MAX_TIMERS (1)

// #define MAX_THREADS (1)

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
    struct itimerspec utmr;
};

// struct platform_thread {
//     pthread_t id;
//     platform_thread_cb *cb;
// };

static struct platform_socket sockets[MAX_SOCKETS] = { 0 };
static struct platform_timer timers[MAX_TIMERS]    = { 0 };

static int g_epoll_initialized = 0;
static int g_epoll_fd = 0;

// type 0 = socket
// type 1 = timer
static void add_platform_fd_to_epoll(int type, int fd, void *ptr)
{
    struct epoll_event event = { 0 };

    if (!g_epoll_initialized) {
        g_epoll_fd = epoll_create1(0);
        g_epoll_initialized = g_epoll_fd >= 0;
        if (!g_epoll_initialized) {
            printf("failed to init epoll\n");
        }
    }

    event.events   = EPOLLIN;
    event.data.fd  = fd;
    // event.data.ptr = ptr;
    // printf("add_platform_fd_to_epoll: %p\n", ptr);
    // if (type == 0) {
    //     for (size_t i = 0; i < MAX_SOCKETS; i += 1) {
    //         if (sockets[i].fd == fd) {
    //             // event.data.ptr = &sockets[i];
    //             event.data.u64 = (u64_t) i;
    //             break;
    //         }
    //     }
    // } else {
    //     // event.data.ptr = &timers[0];
    //     event.data.u64 = 0;
    // }
    // event.data.u32 = (u32_t) type;
    if (epoll_ctl(g_epoll_fd, EPOLL_CTL_ADD, fd, &event) != 0) {
        printf("failed to add fd to epoll\n");
    }
}

// static struct platform_thread threads[MAX_THREADS] = { 0 };

static struct platform_socket *find_free_socket(void)
{
    for (size_t i = 0, max = macro_util_arr_len(sockets); i < max; i += 1) {
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

// struct platform_thread *platform_thread_new(void)
// {
//     return &threads[0];
// }

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

    // src = find_free_socket();
    src->fd          = socket(AF_INET, SOCK_STREAM, 0);
    src->initialized = 1;
    src->is_server   = 1;

    if (src->fd < 0) {
        goto abort;
    }

    // Make the socket reusable.
    reusable_enable = 1;

    if (setsockopt(src->fd,
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
    add_platform_fd_to_epoll(0, src->fd, src);

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

int platform_socket_accept(struct platform_socket *dest,
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
    add_platform_fd_to_epoll(0, dest->fd, dest);

    return 1;
}

int platform_socket_listen(struct platform_socket *src,
                           size_t src_len,
                           platform_socket_request_cb *cb)
{
    static struct epoll_event events[64] = { 0 };
    static byte_t buf[macro_util_mb(5)]  = { 0 };

    struct platform_socket *socket = 0;
    struct platform_timer *timer = 0;

    // struct epoll_event event = { 0 };

    // int epoll_fd = 0;
    int ev_count = 0;

    ssize_t read = 0;

    // if (!src || !src_len || !cb) {
        // return 0;
    // }

    // epoll_fd = epoll_create1(0);

    // if (epoll_fd < 0) {
    //     return 0;
    // }

    while (1) {
        // for (size_t i = 0; i < src_len; i += 1) {
        //     if (!src[i].initialized) {
        //         continue;
        //     }

        //     // EPOLLIN: Ready to read.
        //     // EPOLLOUT: Ready to write.
        //     // DO NOT include EPOLLET, the thing just screws things up.
        //     event.events   = EPOLLIN;
        //     event.data.fd  = src[i].fd;
        //     event.data.ptr = &src[i];

        //     // Duplicates won't be included.
        //     epoll_ctl(epoll_fd, EPOLL_CTL_ADD, src[i].fd, &event);
        // }

        ev_count = epoll_wait(g_epoll_fd, events, macro_util_arr_len(events), -1);

        if (ev_count < 0) {
            close(g_epoll_fd);
            return 0;
        }

        for (int i = 0; i < ev_count; i += 1) {
            if ((events[i].events & EPOLLIN) != EPOLLIN) {
                continue;
            }

            socket = 0;
            timer = 0;

            for (size_t n = 0; n < MAX_SOCKETS; n += 1) {
                if (sockets[n].initialized && sockets[n].fd == events[i].data.fd) {
                    socket = &sockets[n];
                    break;
                }
            }

            if (!socket) {
                timer = &timers[0];
                cb((void *) timer, PLATFORM_SOCKET_TICK, 0, 0);
                // Repeat the timer.
                timerfd_settime(timer->fd, 0, &timer->utmr, 0);
                continue;
            }

            if (!socket->initialized) {
                continue;
            }

            if (socket->is_server) {
                cb(socket, PLATFORM_SOCKET_NEW_CONNECTION, 0, 0);
                continue;
            }

            read = recv(socket->fd, buf, sizeof(buf), 0);

            if (read < 0) {
                cb(socket, PLATFORM_SOCKET_FAILED_TO_READ, 0, 0);
                continue;
            }

            if (read == 0) {
                cb(socket, PLATFORM_SOCKET_DISCONNECTED, 0, 0);
                *socket = (struct platform_socket){ 0 };
                continue;
            }

            cb(socket, PLATFORM_SOCKET_NEW_REQUEST, buf, (size_t) read);
        }
    }

    close(g_epoll_fd);

    return 1;
}

int platform_socket_send(struct platform_socket *dest,
                         ssize_t *sent,
                         void *buf,
                         size_t n)
{
    ssize_t tmp = 0;

    assert(dest);
    assert(sent);

    tmp = send(dest->fd, buf, n, 0);

    if (tmp < 0) {
        return 0;
    }

    *sent = tmp;

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

int platform_timer_init(struct platform_timer *src,
                        u32_t ms_timeout,
                        int repeats)
{
    if (!src) {
        return 0;
    }

    printf("timer[0]: %p, equal? %d\n", &timers[0], &timers[0] == src);
    printf("platform_timer_init: %p\n", src);

    // src = &timers[0];
    src->fd          = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    src->initialized = 1;
    src->repeats     = repeats;
    src->stopped     = 0;

    if (src->fd < 0) {
        *src = (struct platform_timer){ 0 };
        return 0;
    }

    if (ms_timeout >= 1000) {
        src->utmr.it_value.tv_sec    = (long) (ms_timeout / 1000);
        src->utmr.it_interval.tv_sec = src->utmr.it_value.tv_sec;
    } else {
        src->utmr.it_value.tv_nsec    = (long) (ms_timeout * 1000000);
        src->utmr.it_interval.tv_nsec = src->utmr.it_value.tv_nsec;
    }

    if (timerfd_settime(src->fd, 0, &src->utmr, 0) < 0) {
        *src = (struct platform_timer){ 0 };
        return 0;
    }

    add_platform_fd_to_epoll(1, src->fd, src);

    return 1;
}

// int platform_timer_start(struct platform_timer *src,
//                          size_t src_len,
//                          platform_timer_tick_cb *cb)
// {
//     static struct epoll_event events[64] = { 0 };

//     struct epoll_event event = { 0 };

//     int epoll_fd = 0;
//     int ev_count = 0;

//     struct platform_timer *timer = 0;

//     if (!src || !src_len || !cb) {
//         return 0;
//     }

//     epoll_fd = epoll_create1(0);

//     if (epoll_fd < 0) {
//         return 0;
//     }

//     while (1) {
//         for (size_t i = 0; i < src_len; i += 1) {
//             if (!src[i].initialized || src[i].stopped) {
//                 continue;
//             }

//             event.events   = EPOLLIN;
//             event.data.fd  = src[i].fd;
//             event.data.ptr = &src[i];

//             // Duplicates won't be included.
//             epoll_ctl(epoll_fd, EPOLL_CTL_ADD, src[i].fd, &event);
//         }

//         ev_count = epoll_wait(epoll_fd, events, macro_util_arr_len(events), -1);

//         if (ev_count < 0) {
//             close(epoll_fd);
//             return 0;
//         }

//         for (int i = 0; i < ev_count; i += 1) {
//             if ((events[i].events & EPOLLIN) != EPOLLIN) {
//                 continue;
//             }

//             timer = events[i].data.ptr;

//             if (timer->stopped) {
//                 event.data.fd = timer->fd;
//                 epoll_ctl(epoll_fd, EPOLL_CTL_DEL, src[i].fd, &event);
//                 continue;
//             }

//             cb(timer);

//             if (timer->repeats) {
//                 timerfd_settime(timer->fd, 0, &timer->utmr, 0);
//             }
//         }
//     }

//     close(epoll_fd);

//     return 1;
// }

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

/*
 * ---------------------
 * Thread implementation
 * ---------------------
 */

// static void *platform_thread_on_start(void *arg)
// {
//     struct platform_thread *thread = 0;

//     if (!arg) {
//         return 0;
//     }

//     thread = arg;
//     thread->cb(thread);

//     return 0;
// }

// int platform_thread_create(struct platform_thread *thread,
//                            platform_thread_cb *cb)
// {
//     if (!thread || !cb) {
//         return 0;
//     }

//     thread->cb = cb;

//     // Returns non zero if something went wrong.
//     if (pthread_create(&thread->id, 0, platform_thread_on_start, thread)) {
//         return 0;
//     }

//     return 1;
// }

// int platform_thread_kill(struct platform_thread *thread)
// {
//     if (!thread) {
//         return 0;
//     }

//     pthread_exit(0);

//     return 1;
// }

/*
 * -------------------------
 * End thread implementation
 * -------------------------
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
