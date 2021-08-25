#ifndef INCLUDE_OS_IO_H
#define INCLUDE_OS_IO_H

#include "util.h"

typedef enum {
        OS_IO_SOCKET_CONNECTION,
        OS_IO_SOCKET_READ_ERROR,
        OS_IO_SOCKET_REQUEST,
        OS_IO_SOCKET_DISCONNECTED,
        OS_IO_TIMER_TICK,
} os_io_event_t;

struct os_io;

typedef void
os_io_cb(struct os_io *io, os_io_event_t event, void *buf, size_t n);

struct os_io *os_io_socket_create(u16_t port, size_t max_conn);

struct os_io *os_io_timer(double timeout);

int os_io_close(struct os_io *io);

long os_io_write(struct os_io *io, void *buf, size_t n);

// Will block the current executing thread and listen
// for any event from sockets or timers. When a new
// event arrives, cb will be called.
// On error, the function will immediately return 0.
int os_io_listen(os_io_cb *cb);

u32_t os_io_ip_text_to_u32(char *ip);

#endif
