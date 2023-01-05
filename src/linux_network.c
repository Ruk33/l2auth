// required for accept4 to be defined...
#define _GNU_SOURCE

#include <errno.h>      // errno
#include <string.h>     // strcpy, strerror
#include <stddef.h>     // size_t
#include <stdio.h>      // printf
#include <sys/epoll.h>  // epoll_create1, epoll_wait, epoll_event, ...
#include <sys/socket.h> // socket, bind, listen
#include <sys/un.h>     // sockaddr_un
#include <sys/timeb.h>  // ftime
#include <netinet/in.h> // sockaddr_in, INADDR_ANY, htons
#include <unistd.h>     // unlink, close
#include "include/l2auth.h"

#define _abs(x) ((x) < 0 ? (-(x)) : (x))

static void print_err(const char *context)
{
    printf("error (%s): %s\n", context, strerror(errno));
}

int network_port(unsigned short port)
{
    int server = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (server == -1)
        goto abort;
    
    int reuse = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
        goto abort;
    
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server, (struct sockaddr *) &address, sizeof(address)) == -1)
        goto abort;
    
    if (listen(server, SOMAXCONN) == -1)
        goto abort;
    
    return server;
    
abort:
    print_err("asocket_port");
    close(server);
    return -1;
}

int network_sock(char *path)
{
    if (!path) {
        printf("error: no path provided for socket.\n");
        return -1;
    }
    
    int server = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server == -1)
        goto abort;
    
    int reuse = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
        goto abort;
    
    struct sockaddr_un address = {0};
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, path);
    if (unlink(path) == -1)
        goto abort;
    
    if (bind(server, (struct sockaddr *) &address, sizeof(address)) == -1)
        goto abort;
    
    if (listen(server, SOMAXCONN) == -1)
        goto abort;
    
    return server;
    
abort:
    print_err("asocket_sock");
    close(server);
    return -1;
}

void network_listen(int server, network_handler *handler)
{
    static struct epoll_event events[128] = {0};
    static unsigned char read_buf[8192] = {0};
    if (!handler) {
        printf("error: no socket request handler provided.\n");
        return;
    }
    
    int epoll_fd = 0;
    struct epoll_event event = {0};
    int ev_count = 0;
    
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
        goto abort;
    
    event.data.fd = server;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server, &event) == -1)
        goto abort;
    
    struct timeb last_tick = {0};
    while (1) {
        ev_count = epoll_wait(epoll_fd, events, arr_len(events), 30);
        if (ev_count == -1)
            goto abort;

        // tick
        struct timeb now = {0};
        ftime(&now);
        float d = _abs(now.millitm - last_tick.millitm);
        if (d >= 30) {
            last_tick = now;
            d = d > 30 ? 30 : d;
            d /= 1000;
            handler(server, NETWORK_TICK, &d, sizeof(d));
        }

        for (int i = 0; i < ev_count; i += 1) {
            // only read and write, ignore the rest.
            if (!(events[i].events & EPOLLIN) &&
                !(events[i].events & EPOLLOUT))
                continue;
            
            // server
            if (events[i].data.fd == server) {
                while (1) {
                    int client = accept4(server, 0, 0, SOCK_NONBLOCK);
                    if (client == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                        break;
                    if (client == -1) {
                        print_err("failed to accept new client in loop");
                        break;
                    }
                    event.data.fd = client;
                    event.events = EPOLLIN | EPOLLOUT | EPOLLET;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) {
                        print_err("failed to add new client to epoll");
                        close(client);
                    } else {
                        handler(client, NETWORK_NEW_CONN, 0, 0);
                    }
                }
                continue;
            }
            
            // clients.
            if (events[i].events & EPOLLIN) {
                while (1) {
                    ssize_t received = recv(events[i].data.fd, read_buf, sizeof(read_buf), 0);
                    if (received == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                        break;
                    if (received == -1) {
                        print_err("failed while reading");
                        break;
                    }
                    if (received == 0) {
                        handler(events[i].data.fd, NETWORK_CLOSED, 0, 0);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
                        break;
                    }
                    handler(events[i].data.fd, NETWORK_READ, read_buf, (size_t) received);
                }
            }
            if (events[i].events & EPOLLOUT)
                handler(events[i].data.fd, NETWORK_CAN_WRITE, 0, 0);
        }
    }
    
abort:
    print_err("asocket_listen");
    close(epoll_fd);
    close(server);
}

size_t network_write(int socket, void *buf, size_t n)
{
    size_t sent = 0;
    if (!buf)
        return 0;
    while (sent < n) {
        ssize_t tmp = send(socket, buf + sent, n - sent, 0);
        if (tmp == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
            break;
        if (tmp == -1) {
            print_err("asocket_write");
            break;
        }
        sent += tmp;
    }
    return sent;
}

// franco.montenegro: should we just use the functions provided by linux?
int network_ipv4_to_u32(u32 *dest, struct ipv4 *src)
{
    assert(dest);
    assert(src);

    i8 *buf = src->buf;

    for (int i = 0; i < 4; i += 1) {
        u32 tmp = 0;
        for (int n = 0; *buf && *buf != '.' && n < 3; n += 1) {
            // only numbers for ip.
            if (*buf >= '0' && *buf <= '9') {
                tmp = 10 * tmp + *buf - '0';
                buf += 1;
            // anything else, abort.
            } else {
                goto abort;
            }
        }
        ((u8 *) dest)[i] = (u8) tmp;
        if (tmp > 255) {
            goto abort;
        }
        // if the entire IP has been converted just exit.
        if (i == 3) {
            break;
        }
        if (*buf != '.') {
            goto abort;
        }
        // skip dot (.)
        buf += 1;
    }

    return 1;

abort:
    *dest = 0;
    return 0;
}
