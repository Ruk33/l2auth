#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <stddef.h>

/**
 * All requests made by clients is considered
 * work. Every server timer tick is work too.
 * Client requests and timer ticks run
 * in different threads. This work queue is
 * implemented in order to avoid having to
 * lock resources all over the code since
 * work is added safely to the queue and then
 * processed one at a time.
 */

/**
 * Add timer request/work to the queue.
 */
void work_queue_add_timer_request(void *data);

/**
 * Add client request (coming from socket)
 * to the work queue.
 */
void work_queue_add_client_request(
        void *         data,
        int            fd,
        unsigned char *buf,
        size_t         buf_size);

/**
 * Start the work queue.
 * This function will block the thread.
 */
void *work_queue_start(void *p);

#endif
