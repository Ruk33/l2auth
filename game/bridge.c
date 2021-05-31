#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "game_server_lib.h"
#include "work_queue.h"
#include "bridge.h"

typedef enum {
        msg_to_host_log,
        msg_to_host_socket_response,
} msg_to_host_type_t;

typedef struct {
        int    socket;
        void * response;
        size_t size;
} gs_response_msg_t;

static void gs_response(gs_response_msg_t *msg)
{
        work_queue_send_response(msg->socket, msg->response, msg->size);
}

static void msg_to_host(int t, void *payload)
{
        msg_to_host_type_t type = 0;

        type = (msg_to_host_type_t) t;

        switch (type) {
        case msg_to_host_log:
                work_queue_log((char *) payload);
                break;
        case msg_to_host_socket_response:
                gs_response((gs_response_msg_t *) payload);
                break;
        default:
                break;
        }
}

void *bridge_init(void)
{
        void * memory = NULL;
        size_t size   = 0;

        /**
         * TODO: Make allocated mem a configurable
         */
        size   = 1024 * 1024 * 5;
        memory = calloc(1, size);
        assert(memory && "Initial game server memory allocation failed.");

        return game_server_lib_handle_init(memory, size, &msg_to_host);
}
