#ifndef L2AUTH_L2_CLIENT_C
#define L2AUTH_L2_CLIENT_C

#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <log/log.h>
#include <core/circular_memory_alloc.h>
#include <core/l2_socket.h>
#include <core/l2_rsa_key.h>
#include <core/l2_blowfish_key.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/send_packet.h>
#include <packet/server/encrypt.h>
#include <packet/client/decrypt.h>
#include <core/l2_client.h>

struct L2Client* l2_client_new()
{
        struct L2Client* client = calloc(1, sizeof(struct L2Client));

        client->socket = calloc(1, sizeof(struct L2Socket));
        client->rsa_key = l2_rsa_key_new();
        client->blowfish_key = l2_blowfish_key_new();
        client->received_data_size = 0;
        client->received_data = calloc(65535, sizeof(char));
        client->memory = circular_memory_alloc_reserve_space(65535);

        return client;
}

circular_memory_space* l2_client_alloc(struct L2Client* client, size_t how_much)
{
        return circular_memory_alloc(client->memory, how_much);
}

void l2_client_accept
(
        struct L2Client* client,
        struct L2Socket* server
)
{
        if (!client) {
                log_fatal("No client passed when accepting client connection");
                return;
        }
        if (!server) {
                log_fatal("No server passed when accepting client connection");
                return;
        }
        l2_socket_accept(server, client->socket);
        log_info("Connection accepted");
}

void l2_client_close(struct L2Socket* server)
{
        if (!server) {
                log_fatal("No server passed when trying to close client connection");
                return;
        }
        l2_socket_close(server);
        log_info("Connection closed");
}

void l2_client_send_packet
(
        struct L2Client* client,
        l2_raw_packet* packet
)
{
        l2_raw_packet_size packet_size;
        char *printable;

        if (!client) {
                log_fatal("No client passed when trying to send packet");
                return;
        }

        if (!packet) {
                log_error("Trying to send packet to client but none was passed");
                return;
        }

        packet_size = l2_raw_packet_get_size(packet);
        printable = l2_client_alloc(client, packet_size * 3 * sizeof(char));

        send_packet(client->socket, packet);
        log_info("Sent packet");

        log_info("Packet size: %d", packet_size);

        for (int i = 0; i < packet_size; i++)
                sprintf(printable + i * 3, "%02X ", packet[i]);

        log_info("Packet bytes: %s", printable);
}

void l2_client_encrypt_and_send_packet
(
        struct L2Client* client,
        l2_raw_packet* packet
)
{
        l2_raw_packet *encrypted_packet;

        if (!client) {
                log_fatal("No client was passed when trying to encrypt and send packet");
                return;
        }

        if (!packet) {
                log_error("Trying to encrypt and send packet but none was passed");
                return;
        }

        encrypted_packet = packet_server_encrypt(client, packet);
        l2_client_send_packet(client, encrypted_packet);
}

l2_raw_packet* l2_client_wait_packet_for_gameserver
(
        struct L2Client* client
)
{
        if (!client) {
                log_fatal("No client was passed when waiting for packet for gameserver");
                return NULL;
        }

        client->received_data_size = l2_socket_receive(
                client->socket,
                client->received_data,
                L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES
        );

        unsigned char* content_without_size_header =
                client->received_data_size ?
                client->received_data + sizeof(short) :
                client->received_data;
        
        ssize_t content_without_size_header_size = (
                client->received_data_size ?
                (ssize_t) (client->received_data_size - (ssize_t) sizeof(short)) :
                client->received_data_size
        );

        log_info("Received data from client to gameserver");

        return l2_raw_packet_new(
                content_without_size_header,
                (unsigned short) content_without_size_header_size
        );
}

l2_raw_packet* l2_client_wait_and_decrypt_packet
(
        struct L2Client* client
)
{
        if (!client) {
                log_fatal("No client was passed when waiting for packet for loginserver");
                return NULL;
        }

        client->received_data_size = l2_socket_receive(
                client->socket,
                client->received_data,
                L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES
        );

        log_info("Received data from client to loginserver");

        return packet_client_decrypt(
                client->blowfish_key,
                client->received_data,
                (unsigned short) client->received_data_size
        );
}

int l2_client_decrypt_client_packet
(
        struct L2Client* client,
        l2_packet* packet,
        unsigned char* dest
)
{
        unsigned short packet_content_size;
        unsigned char* packet_content;
        int decrypt_result;

        if (!client) {
                log_fatal("No client was passed when trying to decrypt client packet");
                return -1;
        }
        if (!packet) {
                log_error("Trying to decrypt client packet but none was passed");
                return -1;
        }
        if (!dest) {
                log_fatal("Trying to decrypt packet but no destination was passed");
                return -1;
        }

        packet_content_size = l2_packet_get_content_size(packet);
        packet_content = calloc(packet_content_size, sizeof(char));

        l2_packet_content(packet, packet_content, 0, packet_content_size);

        decrypt_result = l2_rsa_key_decrypt(
                client->rsa_key,
                packet_content,
                dest
        );

        free(packet_content);

        return decrypt_result;
}

int l2_client_connection_ended(struct L2Client* client)
{
        return !client || client->received_data_size <= 0;
}

#endif
