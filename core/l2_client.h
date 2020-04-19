#ifndef L2AUTH_L2_CLIENT_H
#define L2AUTH_L2_CLIENT_H

#include <stdlib.h>
#include <sys/types.h>
#include <core/circular_memory_alloc.h>
#include <core/l2_rsa_key.h>
#include <core/l2_blowfish_key.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <os/socket.h>
#include <game/dto/char.h>

#define L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES 65536

struct L2Client;

size_t l2_client_struct_size();

void l2_client_init(struct L2Client* client);

/**
 * Get playable character selected from client
 */
struct GameDtoChar* l2_client_get_char(struct L2Client* client);

struct LoginDtoSessionKey* l2_client_session(struct L2Client* client);

/*
 * (franco.montenegro)
 * Should we really expose the RSA key?
 */
struct L2RSAKey* l2_client_rsa_key(struct L2Client* client);

/*
 * (franco.montenegro)
 * Should we really expose the Blowfish key?
 */
struct L2BlowfishKey* l2_client_blowfish_key(struct L2Client* client);

/**
 * Create a temporary byte builder, useful for composing
 * the content of the packets
 */
byte_builder* l2_client_byte_builder(struct L2Client* client, size_t how_much);

/**
 * Make the client allocate memory. Be sure to free it!
 */
void* l2_client_alloc(struct L2Client* client, size_t how_much);

/**
 * Free memory reserved by l2_client_alloc
 */
void l2_client_alloc_free(struct L2Client* client, void* how_much);

/**
 * Allocate temporary memory that does not
 * need to be free'd but it may be overritten at any time
 */
void* l2_client_alloc_temp_mem(struct L2Client* client, size_t how_much);

/**
 * Create temporary raw packet. Memory does not need to be free
 */
l2_raw_packet* l2_client_create_raw_packet
(
        struct L2Client* client,
        unsigned char* content,
        unsigned short content_size
);

/**
 * Create temporary packet. Memory does not need to be free
 */
l2_packet* l2_client_create_packet
(
        struct L2Client* client,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
);

void l2_client_accept
(
        struct L2Client* client,
        os_socket_handler* server_socket
);

void l2_client_close(struct L2Client* client);

int l2_client_connection_ended(struct L2Client* client);

void l2_client_send_packet(struct L2Client* client, l2_raw_packet* packet);

void l2_client_encrypt_and_send_packet
(
        struct L2Client* client,
        l2_raw_packet* packet
);

/**
 * Block execution until we get a packet from the client
 */
l2_raw_packet* l2_client_wait_packet(struct L2Client* client);

/**
 * Block execution until we get a packet and decrypt it
 */
l2_raw_packet* l2_client_wait_and_decrypt_packet(struct L2Client* client);

int l2_client_decrypt_client_packet
(
        struct L2Client* client,
        l2_packet* packet,
        unsigned char* dest
);

#endif
