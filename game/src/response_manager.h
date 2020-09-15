#ifndef RESPONSE_MANAGER_H
#define RESPONSE_MANAGER_H

#include <stdlib.h>

void response_manager_init
(void);

void response_manager_enqueue
(int client_id, unsigned char *buf, size_t buf_size);

void *response_manager_start_handling_responses
(void *p);

#endif
