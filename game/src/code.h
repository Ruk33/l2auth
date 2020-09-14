#ifndef CODE_H
#define CODE_H

#include <stdlib.h>
#include <os/memory.h>
#include <os/socket.h>

struct Code;

typedef void (*send_reponse_cb)
(int client_id, unsigned char *buf, size_t buf_size);

void code_init
(memory *mem);

void code_new_conn
(void *server_data, int client_id);

void *code_malloc
(size_t how_much);

void code_mfree
(void *memory);

void *code_initialize_server
(send_reponse_cb s);

void code_request_handle
(void *server_data, int client_id, unsigned char *buf, size_t buf_size);

void code_handle_disconnect
(void *server_data, int client_id);

#endif
