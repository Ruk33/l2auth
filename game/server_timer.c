#include <stdio.h>
#include "timer.h"
#include "work_queue.h"
#include "server.h"
#include "server_timer.h"

/**
 * TODO: Implement
 */

static void tick(void)
{
        void *data = NULL; // server_data();

        if (!data) {
                printf("Server timer: server data not found\n");
                return;
        }

        work_queue_add_timer_request(data);
}

void *server_timer_start(void *p)
{
        timer_start(1, tick);
        return p;
}
