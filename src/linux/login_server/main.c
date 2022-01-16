#include <assert.h>
#include <stdio.h>

#include "../../include/util.h"
#include "../../include/config.h"
#include "../../include/ls_types.h"

#include "../platform.c"
#include "../../ls_lib.c"

static struct platform_socket *g_sockets[MAX_CLIENTS] = { 0 };
static size_t g_socket_instances[MAX_CLIENTS]         = { 0 };

static void send_response(struct platform_socket *dest, void *buf, size_t n)
{
    ssize_t sent = 0;

    if (!dest) {
        return;
    }

    if (platform_socket_send(dest, &sent, buf, n)) {
        return;
    }

    printf("unable to send response.\n");
}

static void disconnect(struct platform_socket *src)
{
    if (!src) {
        return;
    }

    if (platform_socket_close(src)) {
        return;
    }

    printf("unable to close connection.\n");
}

static u32_t ip_to_u32(char *ip)
{
    u32_t result = 0;

    if (!ip) {
        return 0;
    }

    if (platform_ip_to_u32(&result, ip)) {
        return result;
    }

    printf("unable to convert ip '%s' to u32.\n", ip);

    return 0;
}

static void on_request(struct platform_socket *src,
                       enum platform_socket_request_type type,
                       void *buf,
                       size_t n)
{
    size_t free_socket = 0;

    if (!src) {
        return;
    }

    switch (type) {
    case PLATFORM_SOCKET_NEW_CONNECTION:
        util_recycle_id_get(&free_socket, g_socket_instances);
        g_sockets[free_socket] = platform_socket_new();

        if (!g_sockets[free_socket]) {
            util_recycle_id(g_socket_instances, free_socket);
            printf(
                "login server unable to get new socket for accepting "
                "client.\n");
            return;
        }

        if (!platform_socket_accept(g_sockets[free_socket], src)) {
            util_recycle_id(g_socket_instances, free_socket);
            printf("login server unable to accept new connection.\n");
            return;
        }

        ls_lib_new_conn(g_sockets[free_socket]);
        break;
    case PLATFORM_SOCKET_NEW_REQUEST:
        ls_lib_new_req(src, buf, n);
        break;
    case PLATFORM_SOCKET_FAILED_TO_READ:
        printf("login server failed to read packet.\n");
        break;
    case PLATFORM_SOCKET_READY_TO_WRITE:
        printf("login server ready to write.\n");
        break;
    case PLATFORM_SOCKET_DISCONNECTED:
        printf("login server client disconnected.\n");
        for (size_t i = 0; i < macro_util_arr_len(g_sockets); i += 1) {
            if (g_sockets[i] == src) {
                util_recycle_id(g_socket_instances, i);
                break;
            }
        }
        break;
    default:
        break;
    }
}

int main(/* int argc, char **argv */)
{
    static struct ls_state ls = { 0 };

    size_t free_socket = 0;

    util_recycle_id_get(&free_socket, g_socket_instances);
    g_sockets[free_socket] = platform_socket_new();

    if (!g_sockets[free_socket]) {
        printf("login server socket couldn't be created.\n");
        return 1;
    }

    if (!platform_socket_init(g_sockets[free_socket], 2106, MAX_CLIENTS)) {
        printf("login server socket unable to initialize.\n");
        return 1;
    }

    ls.send_response  = send_response;
    ls.disconnect     = disconnect;
    ls.text_ip_to_u32 = ip_to_u32;

    ls_lib_load(&ls);

    printf("login server started.\n");

    if (!platform_socket_listen(*g_sockets,
                                macro_util_arr_len(g_sockets),
                                on_request)) {
        printf("login server socket unable to listen for new connections.\n");
        return 1;
    }

    return 0;
}
