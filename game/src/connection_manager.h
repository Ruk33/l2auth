#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <stdlib.h>
#include <os/socket.h>

/**
 * NOTE, make sure to call it before using this manager
 */
void connection_manager_init
(size_t max_players);

void connection_manager_new_conn
(void *server_data, os_socket_handler *conn_socket);

void connection_manager_send_response
(int conn_id, unsigned char *buf, size_t buf_size);

#endif
