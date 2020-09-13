#ifndef RESPONSE_QUEUE_H
#define RESPONSE_QUEUE_H

#include <stdlib.h>

void response_queue_init
(void);

void response_queue_enqueue
(int client_id, unsigned char *buf, size_t buf_size);

void *response_queue_start_handling_responses
(void *p);

#endif
