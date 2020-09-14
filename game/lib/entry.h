#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include "host.h"

void *entry_initialize_server
(host_malloc_cb m, host_mfree_cb f, host_send_response_cb s);

void entry_new_conn
(void *server_data, int client_id);

void entry_handle_request
(void *server_data, int client_id, unsigned char *buf, size_t buf_size);

void entry_handle_disconnect
(void *server_data, int client_id);

#endif
