#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <core/l2_raw_packet.h>
#include <core/session_key.h>
#include "../shared/l2_server.h"
#include "../shared/client_id.h"
#include "dto/character.h"
#include "dto/pc.h"

struct Client;
typedef void (* request_handler)(struct Client *client, l2_raw_packet *packet);

struct Client *client_new(struct L2Server *l2_server, client_id id);

void *client_alloc_mem(struct Client *client, size_t how_much);
void client_free_mem(struct Client *client, void *mem);

void client_handle_request(struct Client *client, char *request, size_t request_size);
void client_queue_response(struct Client *client, l2_raw_packet *response);
void client_update_request_handler(struct Client *client, request_handler handler);

void client_update_session(struct Client *client, struct L2SessionKey *session);
struct L2SessionKey *client_session(struct Client *client);

void client_encrypt_packet(struct Client *client, l2_raw_packet *packet);
void client_decrypt_packet(struct Client *client, l2_raw_packet *packet);

void client_update_character(struct Client *client, struct Pc *character);
struct Character *client_character(struct Client *client, int obj_id);
struct Pc *client_player(struct Client *client);

void client_handle_disconnect(struct Client *client);

#endif
