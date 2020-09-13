#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H

#include <stdlib.h>

void request_queue_init
(void);

void request_queue_enqueue
(void *server_data, int client_id, unsigned char *buf, size_t buf_size);

void *request_queue_start_handling_requests
(void *p);

#endif
