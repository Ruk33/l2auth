#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "server.h"
#include "memory.h"

int main(/* int arc, char **argv */)
{
        unsigned short port = 7777;
        size_t max_connections = 10;

        if (!memory_init()) {
                printf("Failed to allocate memory.\n");
                return EXIT_FAILURE;
        }

        return server_start(port, max_connections);
}
