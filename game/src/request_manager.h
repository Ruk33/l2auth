#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <stdlib.h>

void request_manager_init
(void);

void request_manager_enqueue
(void *server_data, int client_id, unsigned char *buf, size_t buf_size);

void *request_manager_start_handling_requests
(void *p);

#endif
