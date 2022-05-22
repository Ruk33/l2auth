#ifndef CLIENT_H
#define CLIENT_H

#include "../../../include/util.h"
#include "../../../include/packet.h"
#include "types.h"

void client_init(struct client *src);

void client_decrypt(struct client *client, struct packet *src);

void client_encrypt(struct client *client, struct packet *src);

void client_queue_response(struct client *client, struct packet *src);

#endif
