#include <stdlib.h>
#include <pthread.h>
#include <log/log.h>
#include <os/memory.h>
#include "code.h"
#include "request_manager.h"
#include "response_manager.h"
#include "connection_manager.h"
#include "server_manager.h"

#define KB(amount) ((amount) * 1024)
#define MB(amount) ((KB(amount)) * 1024)

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
        reserved_memory = MB(10);
        mem = calloc(1, reserved_memory);

        if (!mem) {
                log_fatal("Failed to allocate initial block of memory");
                return EXIT_FAILURE;
        }

        memory_init(mem, reserved_memory);
        code_init(mem);

        request_manager_init();
        response_manager_init();

        connection_manager_init(max_players);
        server_manager_init(port, max_players);

        pthread_create(
                &requests_thread,
                NULL,
                &request_manager_start_handling_requests,
                NULL
        );
        
        pthread_create(
                &responses_thread,
                NULL,
                &response_manager_start_handling_responses,
                NULL
        );

        server_manager_start(NULL);

        return EXIT_SUCCESS;
}
