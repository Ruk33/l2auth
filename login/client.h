#ifndef L2AUTH_LOGIN_CLIENT_H
#define L2AUTH_LOGIN_CLIENT_H

#include <core/session_key.h>
#include <core/l2_rsa_key.h>
#include <core/l2_blowfish_key.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <os/socket.h>

struct LoginClient;

/**
 * Initialize login client. If we fail to initialize the client
 * (maybe there is no memory left), 0 will be returned
 */
int login_client_init(struct LoginClient* client);

struct L2SessionKey* login_client_session(struct LoginClient* client);

struct L2RSAKey* login_client_rsa_key(struct LoginClient* client);

struct L2BlowfishKey* login_client_blowfish_key(struct LoginClient* client);

void login_client_accept(struct LoginClient* client, os_socket_handler* server_socket);

void login_client_close(struct LoginClient* client);

void login_client_send_packet(struct LoginClient* client, l2_raw_packet* packet);

/**
 * Encrypt and send a packet. Keep in mind @packet will be mutated!
 */
void login_client_encrypt_and_send_packet(struct LoginClient* client, l2_raw_packet* packet);

/**
 * Block execution until a new packet arrives
 */
l2_raw_packet* login_client_wait_packet(struct LoginClient* client);

/**
 * Block execution until a new packet arrives. Once it does, decrypt it
 */
l2_raw_packet* login_client_wait_and_decrypt_packet(struct LoginClient* client);

/**
 * Allocate temporary memory
 */
void* login_client_alloc_temp_mem(struct LoginClient* client, size_t how_much);

byte_builder* login_client_byte_builder(struct LoginClient* client, size_t how_much);

/**
 * Check if the client just closed the connection
 */
int login_client_connection_ended(struct LoginClient* client);

l2_raw_packet* login_client_create_raw_packet
(
        struct LoginClient* client,
        unsigned char* content,
        unsigned short content_size
);

l2_packet* login_client_create_packet
(
        struct LoginClient* client,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
);

size_t login_client_struct_size();

#endif
