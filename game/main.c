#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "bridge.h"
#include "server.h"
#include "work_queue.h"

int main(/* int arc, char **argv */)
{
        void *data = NULL;

        pthread_t work_thread   = 0;
        pthread_t socket_thread = 0;

        data = bridge_init();

        /**
         * NOTE: Should we use only one thread for
         * everything and take work from q'?
         */
        pthread_create(&work_thread, NULL, &work_queue_start, NULL);
        pthread_create(&socket_thread, NULL, &server_start, data);

        pthread_join(work_thread, NULL);
        pthread_join(socket_thread, NULL);

        return 0;
}
