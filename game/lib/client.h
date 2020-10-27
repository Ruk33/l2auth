#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <core/l2_raw_packet.h>
#include <core/session_key.h>
#include "host.h"
#include "dto/player.h"

struct Client;

struct Client *client_new(int id, host_malloc_cb m, host_mfree_cb f, host_send_response_cb s);

/**
 * Make the client alloc how_much of memory. It must be deallocated
 * with client_free_mem
 */
void *client_alloc_mem(struct Client *client, size_t how_much);

/**
 * Deallocate memory allocated with client_alloc_mem
 */
void client_free_mem(struct Client *client, void *mem);

/**
 * Queue a new response/packet to be flushed
 */
void client_queue_response(struct Client *client, l2_raw_packet *response);

void client_update_session(struct Client *client, struct L2SessionKey *session);

/**
 * Get a copy of the session key. It must be deallocated.
 */
struct L2SessionKey *client_session(struct Client *client);

/**
 * Encrypt the packet (it will be modified)
 */
void client_encrypt_packet(struct Client *client, l2_raw_packet *packet);

/**
 * Decrypt the packet (it will be modified)
 */
void client_decrypt_packet(struct Client *client, l2_raw_packet *packet);

void client_update_character(struct Client *client, struct Player *character);

/**
 * Get a copy of the client's player. It must be deallocated.
 */
struct Player *client_player(struct Client *client);

void client_handle_disconnect(struct Client *client);

/**
 * Check if the connection is encrypted
 */
int client_is_conn_encrypted(struct Client *client);

/**
 * Mark the connection as encrypted
 */
void client_encrypt_conn(struct Client *client);

/**
 * Get unique client id
 */
int client_id(struct Client *client);

/**
 * Mark the client as inside of the game
 * (meaning it has a char active in the world)
 */
void client_enter_game(struct Client *client);

/**
 * Check if the client has a char
 * in the world
 */
int client_is_in_game(struct Client *client);

/**
 * Create a copy of client
 */
struct Client *client_copy(struct Client *client);

#endif
