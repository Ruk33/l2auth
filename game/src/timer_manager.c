#include <time.h>
#include "game_server_lib.h"
#include "server_manager.h"
#include "timer_manager.h"

void timer_manager_init(void)
{
}

static void tick(int ms)
{
        clock_t time_delay = ms + clock();
        while (time_delay > clock())
                ;
}

void *timer_manager_start(void *server_data)
{
        // while (1)
        // {
        //         tick(1000);
        //         game_server_lib_tick_server(server_get_data());
        // }

        return NULL;
}
