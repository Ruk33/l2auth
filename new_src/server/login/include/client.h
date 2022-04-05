#ifndef CLIENT_H
#define CLIENT_H

#include "../../../include/util.h"
#include "../../../include/packet.h"

struct client_username {
    i8 buf[16];
};

struct client_request {
    struct packet packet;
    size_t received;
    int is_partial;
};

struct client {
    struct client_username username;
    struct client_request request;
    struct packet response;
};

// On success, 1 is returned.
// On error, 0 is returned.
int client_init(struct client *src);

// Release resources used by client
// so it can be re-used by another connection.
void client_free(struct client *src);

// On success, 1 is returned.
// On error, 0 is returned.
int client_rsa_modulus(struct client *src, struct buffer *dest);

// On success, 1 is returned.
// On error, 0 is returned.
int client_encrypt_packet(struct client *client,
                          struct buffer *dest,
                          struct packet *src);

// On success, 1 is returned.
// On error, 0 is returned.
int client_decrypt_packet(struct client *client,
                          struct packet *dest,
                          struct packet *src);

#endif