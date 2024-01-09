// required for accept4 to be defined...
#define _GNU_SOURCE

#include <errno.h>      // errno
#include <string.h>     // strcpy, strerror
#include <stddef.h>     // size_t
#include <stdio.h>      // printf
#include <sys/epoll.h>  // epoll_create1, epoll_wait, epoll_event, ...
#include <sys/socket.h> // socket, bind, listen, send
#include <sys/un.h>     // sockaddr_un
#include <netinet/in.h> // sockaddr_in, INADDR_ANY, htons
#include <unistd.h>     // unlink, close
#include "asocket.h"

static void print_err(const char *context)
{
    printf("asocket error (%s): %s\n", context, strerror(errno));
}

int asocket_port(unsigned short port)
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

int asocket_sock(char *path)
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

void asocket_listen(int server, asocket_handler *handler)
{
    static struct epoll_event events[128] = {0};
    static unsigned char read_buf[8192] = {0};
    
    if (!handler) {
        printf("asocket error: no socket request handler provided.\n");
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
    
    while (1) {
        ev_count = epoll_wait(epoll_fd, events, sizeof(events) / sizeof(*events), -1);
        if (ev_count == -1)
            goto abort;
        
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
                        handler(client, ASOCKET_NEW_CONN, 0, 0);
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
                        handler(events[i].data.fd, ASOCKET_CLOSED, 0, 0);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
                        break;
                    }
                    handler(events[i].data.fd, ASOCKET_READ, read_buf, (size_t) received);
                }
            }
            if (events[i].events & EPOLLOUT)
                handler(events[i].data.fd, ASOCKET_CAN_WRITE, 0, 0);
        }
    }
    
    abort:
    print_err("asocket_listen");
    close(epoll_fd);
    close(server);
}

unsigned long long asocket_write(int socket, void *buf, unsigned long long n)
{
    unsigned long long sent = 0;
    if (!buf)
        return 0;
    while (sent < n) {
        long long tmp = send(socket, buf + sent, n - sent, 0);
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

void asocket_close(int socket)
{
    close(socket);
}
