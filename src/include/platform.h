#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "util.h"

enum platform_socket_request_type {
        PLATFORM_SOCKET_NEW_CONNECTION,
        PLATFORM_SOCKET_NEW_REQUEST,
        PLATFORM_SOCKET_FAILED_TO_READ,
        PLATFORM_SOCKET_READY_TO_WRITE,
        PLATFORM_SOCKET_DISCONNECTED,
};

struct platform_socket;

struct platform_timer;

typedef void platform_socket_request_cb(
        struct platform_socket *src,
        enum platform_socket_request_type type,
        void *buf,
        u32_t n);

typedef void platform_timer_tick_cb(struct platform_timer *src);

/**
 * Initialize socket.
 * On success, 1 is returned, otherwise, 0.
 */
int platform_socket_init(struct platform_socket *src, u16_t port, u32_t max);

/**
 * Closes socket connection. No writting or reading happens after this call.
 * On success, 1 is returned, otherwise, 0.
 */
int platform_socket_close(struct platform_socket *src);

/**
 * Accept new socket connection and stores it in dest.
 * On success, 1 is returned, otherwise, 0.
 */
int platform_socket_accept(
        struct platform_socket *dest,
        struct platform_socket *src);

/**
 * Listen for requests in all the sockets stored in src and execute cb when a
 * new request comes. On success, it will block the thread. If all connections
 * are closed, the function will return 1. On error, 0 is immediately returned.
 */
int platform_socket_listen(
        struct platform_socket *src,
        u32_t src_len,
        platform_socket_request_cb *cb);

/**
 * Send n bytes of buf to dest.
 * Sent bytes are stored in sent.
 * On success, 1 is returned, otherwise, 0.
 */
int platform_socket_send(
        struct platform_socket *dest,
        u32_t *sent,
        void *buf,
        u32_t n);

/**
 * Initializes timer.
 * On success, 1 will be returned, otherwise, 0.
 */
int platform_timer_init(
        struct platform_timer *src,
        u32_t ms_timeout,
        int repeats);

/**
 * Start all timers stored in src and execute cb when any
 * of the timers expire.
 * On success, the thread will be blocked and cb will be executed with expired
 * timers. On error, 0 will be immediately returned.
 */
int platform_timer_start(
        struct platform_timer *src,
        u32_t src_len,
        platform_timer_tick_cb *cb);

void platform_timer_stop(struct platform_timer *src);

void platform_timer_resume(struct platform_timer *src);

/**
 * Simple message logging.
 */
void platform_normal_log(char *format, ...);

#endif
