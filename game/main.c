#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "server.h"
#include "memory.h"

int main(/* int arc, char **argv */)
{
        unsigned short port = 7777;
        size_t max_connections = 10;

        // pthread_t responses_thread = 0;
        // pthread_t requests_thread = 0;

        // pthread_create(&responses_thread, NULL, &response_manager_init, NULL);
        // pthread_create(&requests_thread, NULL, &request_manager_init, NULL);

        if (!memory_init()) {
                printf("Failed to allocate memory.\n");
                return EXIT_FAILURE;
        }

        return server_start(port, max_connections);
}
