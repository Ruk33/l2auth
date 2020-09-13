#include <stdlib.h>
#include <pthread.h>
#include <log/log.h>
#include <os/memory.h>
#include "code.h"
#include "request_queue.h"
#include "response_queue.h"
#include "connection_manager.h"
#include "server.h"

int main
(int argc, char **argv)
{
        pthread_t requests_thread;
        pthread_t responses_thread;

        unsigned short port = 0;
        size_t max_players = 0;

        size_t reserved_memory = 0;
        memory *mem = NULL;

        port = 7777;
        max_players = 10;
        reserved_memory = 1024 * 1024 * 10;
        mem = calloc(1, reserved_memory);

        if (!mem) {
                log_fatal("Failed to allocate initial block of memory");
                return EXIT_FAILURE;
        }

        memory_init(mem, reserved_memory);
        code_init(mem);

        request_queue_init();
        response_queue_init();

        connection_manager_init(max_players);
        server_init(port, max_players);

        pthread_create(
                &requests_thread,
                NULL,
                &request_queue_start_handling_requests,
                NULL
        );
        
        pthread_create(
                &responses_thread,
                NULL,
                &response_queue_start_handling_responses,
                NULL
        );

        server_start(NULL);

        return EXIT_SUCCESS;
}
