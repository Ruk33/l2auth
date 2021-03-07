#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "server.h"
#include "server_timer.h"
#include "work_queue.h"
#include "memory.h"

int main(/* int arc, char **argv */)
{
        unsigned short port            = 7777;
        size_t         max_connections = 10;

        pthread_t work_thread  = 0;
        pthread_t timer_thread = 0;

        if (!memory_init()) {
                printf("Failed to allocate memory.\n");
                return EXIT_FAILURE;
        }

        pthread_create(&work_thread, NULL, &work_queue_start, NULL);
        pthread_create(&timer_thread, NULL, &server_timer_start, NULL);

        return server_start(port, max_connections);
}
