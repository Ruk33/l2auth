#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>
#include <core/l2_raw_packet.h>
#include "client.h"
#include "host.h"

struct World;

/**
 * Create a new world and return it. It will later be used
 * for all the world_ functions
 */
void *world_new(host_malloc_cb m, host_mfree_cb f, host_send_response_cb s);

/**
 * Handle new client being connected
 */
void world_new_client(void *world_p, int client_id);

/**
 * Handle request made by client
 */
void world_client_request(void *world_p, int client_id, unsigned char *buf, size_t buf_size);

/**
 * Handle client disconnection
 */
void world_client_disconnect(void *world_p, int client_id);

#endif
