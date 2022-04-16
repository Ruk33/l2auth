#ifndef CLIENT_H
#define CLIENT_H

#include "../../../include/util.h"
#include "../../../include/packet.h"

struct client_modulus {
    byte buf[128];
};

struct client_username {
    i8 buf[16];
};

struct client_request {
    struct packet packet;
    size_t received;
    int is_partial;
};

struct client {
    size_t id;
    struct client_username username;
    struct client_request request;
    struct packet response;
    i32 playOK1;
    i32 playOK2;
};

// On success, 1 is returned.
// On error, 0 is returned.
int client_init(struct client *src);

// Release resources used by client
// so it can be re-used by another connection.
void client_free(struct client *src);

// On success, 1 is returned.
// On error, 0 is returned.
int client_rsa_modulus(struct client *src, struct client_modulus *dest);

// Generate ok keys 1 and 2 for the client.
void client_gen_ok_ids(struct client *src);

// On success, 1 is returned.
// On error, 0 is returned.
int client_encrypt_packet(struct client *client, struct packet *src);

// On success, 1 is returned.
// On error, 0 is returned.
int client_decrypt_packet(struct client *client, struct packet *src);

#endif