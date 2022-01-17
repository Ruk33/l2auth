#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "util.h"

enum platform_socket_request_type
{
    PLATFORM_SOCKET_NEW_CONNECTION,
    PLATFORM_SOCKET_NEW_REQUEST,
    PLATFORM_SOCKET_FAILED_TO_READ,
    PLATFORM_SOCKET_READY_TO_WRITE,
    PLATFORM_SOCKET_DISCONNECTED,
    PLATFORM_SOCKET_TICK,
};

struct platform_socket;

struct platform_timer;

// struct platform_thread;

typedef void platform_socket_request_cb(struct platform_socket *src,
                                        enum platform_socket_request_type type,
                                        void *buf,
                                        size_t n);

typedef void platform_timer_tick_cb(struct platform_timer *src);

// typedef void platform_thread_cb(struct platform_thread *thread);

/**
 * Get a new usable socket.
 * On success, a new instance is returned.
 * On error, NULL will be returned.
 */
struct platform_socket *platform_socket_new(void);

/**
 * Get a new usable socket.
 * On success, a new instance is returned.
 * On error, NULL will be returned.
 */
struct platform_timer *platform_timer_new(void);

/**
 * Get a new usable thread.
 * On success, a new instance is returned.
 * ON error, NULL will be returned.
 */
// struct platform_thread *platform_thread_new(void);

void platform_socket_free(struct platform_socket *src);

void platform_timer_free(struct platform_timer *src);

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
int platform_socket_accept(struct platform_socket *dest,
                           struct platform_socket *src);

/**
 * Listen for requests in all the sockets stored in src and execute cb when a
 * new request comes. On success, it will block the thread. If all connections
 * are closed, the function will return 1. On error, 0 is immediately returned.
 */
int platform_socket_listen(struct platform_socket *src,
                           size_t src_len,
                           platform_socket_request_cb *cb);

/**
 * Send n bytes of buf to dest.
 * Sent bytes are stored in sent.
 * On success, 1 is returned, otherwise, 0.
 */
int platform_socket_send(struct platform_socket *dest,
                         ssize_t *sent,
                         void *buf,
                         size_t n);

/**
 * Initializes timer.
 * On success, 1 will be returned, otherwise, 0.
 */
int platform_timer_init(struct platform_timer *src,
                        u32_t ms_timeout,
                        int repeats);

/**
 * Start all timers stored in src and execute cb when any
 * of the timers expire.
 * On success, the thread will be blocked and cb will be executed with expired
 * timers. On error, 0 will be immediately returned.
 */
int platform_timer_start(struct platform_timer *src,
                         size_t src_len,
                         platform_timer_tick_cb *cb);

void platform_timer_stop(struct platform_timer *src);

void platform_timer_resume(struct platform_timer *src);

// int platform_thread_create(struct platform_thread *thread,
//                            platform_thread_cb *cb);

// int platform_thread_kill(struct platform_thread *thread);

/**
 * Simple message logging.
 */
void platform_normal_log(char *format, ...);

/**
 * Convert ip to unsigned int.
 * On success, 1 will be returned and result stored in dest.
 * On error, 0.
 */
int platform_ip_to_u32(u32_t *dest, char *ip);

#endif
