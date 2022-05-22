#ifndef CLIENT_H
#define CLIENT_H

#include "../../../include/util.h"
#include "../../../include/packet.h"

struct key {
	byte buf[8];
};

struct client {
	size_t id;
	struct character *character;
	struct key encrypt_key;
	struct key decrypt_key;
	struct packet response;
	struct packet request;
	size_t received;
	int partial;
	int conn_encrypted;
};

void client_init(struct client *src);

void client_decrypt(struct client *client, struct packet *src);

void client_encrypt(struct client *client, struct packet *src);

#endif
