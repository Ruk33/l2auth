#include <stdlib.h>
#include <pthread.h>
#include <log/log.h>
#include "memory_manager.h"
#include "game_server_lib.h"
#include "request_manager.h"
#include "response_manager.h"
#include "connection_manager.h"
#include "server_manager.h"

int main
(int argc, char **argv)
{
        pthread_t requests_thread = 0;
        pthread_t responses_thread = 0;

        unsigned short port = 0;
        size_t max_players = 0;

        port = 7777;
        max_players = 10;

        memory_manager_init();
        game_server_lib_init();

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

        game_server_lib_finish();
        connection_manager_finish();
        memory_manager_finish();

        return EXIT_SUCCESS;
}
