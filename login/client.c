#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <core/session_key.h>
#include <core/l2_rsa_key.h>
#include <core/l2_blowfish_key.h>
#include <core/l2_raw_packet.h>
#include <core/circular_memory_alloc.h>
#include <core/byte_builder.h>
#include <os/socket.h>
#include <login/packet/blowfish_encrypt.h>
#include <login/packet/blowfish_decrypt.h>
#include "client.h"

#define MAX_DATA_FROM_PACKET 65536

// TEMP_MEM_PER_CLIENT = 65536 * 2
// So basically we got up to two entire packets as temp memory
#define TEMP_MEM_PER_CLIENT 131072

struct LoginClient
{
        os_socket_handler* socket_handler;
        struct L2RSAKey* rsa_key;
        struct L2BlowfishKey* blowfish_key;
        struct L2SessionKey session;
        int rsa_decrypted;

        size_t received_data_size;
        unsigned char received_data[MAX_DATA_FROM_PACKET];

        circular_memory_space temp_memory[TEMP_MEM_PER_CLIENT];
};

int login_client_init(struct LoginClient* client)
{
        assert(client);

        size_t rsa_key_struct_size = l2_rsa_key_struct_size();
        size_t blowfish_struct_size = l2_blowfish_key_struct_size();
        size_t socket_handler_size = os_socket_handler_size();

        client->received_data_size = 0;
        client->rsa_key = malloc(rsa_key_struct_size);
        client->blowfish_key = malloc(blowfish_struct_size);
        client->socket_handler = malloc(socket_handler_size);

        if (!client->rsa_key || !client->blowfish_key || !client->socket_handler) {
                login_client_close(client);
                return 0;
        }

        l2_rsa_key_init(client->rsa_key);
        l2_blowfish_key_init(client->blowfish_key);
        l2_session_key_init(&client->session);
        circular_memory_alloc_init(client->temp_memory, TEMP_MEM_PER_CLIENT);

        return 1;
}

struct L2SessionKey* login_client_session(struct LoginClient* client)
{
        assert(client);
        return &client->session;
}

struct L2RSAKey* login_client_rsa_key(struct LoginClient* client)
{
        assert(client);
        return client->rsa_key;
}

struct L2BlowfishKey* login_client_blowfish_key(struct LoginClient* client)
{
        assert(client);
        return client->blowfish_key;
}

void login_client_accept(struct LoginClient* client, os_socket_handler* server_socket)
{
        assert(client);
        assert(server_socket);
        os_socket_accept(server_socket, client->socket_handler);
        log_info("Connection accepted");
}

void login_client_close(struct LoginClient* client)
{
        assert(client);

        if (client->rsa_key) free(client->rsa_key);
        if (client->blowfish_key) free(client->blowfish_key);

        if (client->socket_handler) {
                os_socket_close(client->socket_handler);
                free(client->socket_handler);
        }

        log_info("Connection closed");
}

void login_client_send_packet(struct LoginClient* client, l2_raw_packet* packet)
{
        assert(client);
        assert(packet);

        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        char *printable = login_client_alloc_temp_mem(
                client,
                (size_t) packet_size * 3 * sizeof(char)
        );

        os_socket_send(
                client->socket_handler,
                packet,
                l2_raw_packet_get_size(packet)
        );

        log_info("Sent packet");
        log_info("Packet size: %d", packet_size);

        for (int i = 0; i < packet_size; i++)
                sprintf(printable + i * 3, "%02X ", packet[i]);

        log_info("Packet bytes: %s", printable);
}

void login_client_encrypt_and_send_packet(struct LoginClient* client, l2_raw_packet* packet)
{
        assert(client);
        assert(packet);

        l2_raw_packet *encrypted_packet = login_packet_blowfish_encrypt(client, packet);
        login_client_send_packet(client, encrypted_packet);
}

l2_raw_packet* login_client_wait_packet(struct LoginClient* client)
{
        assert(client);

        unsigned char* content_without_size_header = NULL;

        client->received_data_size = os_socket_receive(
                client->socket_handler,
                client->received_data,
                MAX_DATA_FROM_PACKET
        );

        if (client->received_data_size == 0) {
                return NULL;
        }

        content_without_size_header =
                client->received_data_size ?
                client->received_data + sizeof(l2_raw_packet_size) :
                client->received_data;
        
        size_t content_without_size_header_size = (
                client->received_data_size ?
                client->received_data_size - sizeof(short) :
                client->received_data_size
        );

        l2_raw_packet* packet = login_client_alloc_temp_mem(
                client,
                l2_raw_packet_calculate_size((unsigned short) content_without_size_header_size)
        );

        l2_raw_packet_init(
                packet,
                content_without_size_header,
                (unsigned short) content_without_size_header_size
        );

        return packet;
}

l2_raw_packet* login_client_decrypt_blowfish
(
        struct LoginClient* client,
        unsigned char* data,
        unsigned short data_len
)
{
        assert(client);
        assert(data);
        assert(data_len);
        return login_packet_blowfish_decrypt(client, data, data_len);
}

int login_client_decrypt_rsa
(
        struct LoginClient* client,
        l2_packet* packet,
        unsigned char* dest
)
{
        assert(client);
        assert(packet);
        assert(dest);

        unsigned short packet_content_size =
                l2_packet_get_content_size(packet);
        unsigned char* packet_content = login_client_alloc_temp_mem(
                client,
                packet_content_size * sizeof(char)
        );
        int decrypt_result;

        l2_packet_content(packet, packet_content, 0, packet_content_size);

        decrypt_result = l2_rsa_key_decrypt(
                client->rsa_key,
                packet_content,
                dest
        );

        return decrypt_result;
}

l2_raw_packet* login_client_wait_and_decrypt_packet(struct LoginClient* client)
{
        assert(client);

        l2_raw_packet* packet_blowfish_decrypted;
        l2_raw_packet* decrypted_packet;

        client->received_data_size = os_socket_receive(
                client->socket_handler,
                client->received_data,
                MAX_DATA_FROM_PACKET
        );

        if (client->received_data_size == 0) {
                return NULL;
        }

        packet_blowfish_decrypted = login_client_decrypt_blowfish(
                client,
                client->received_data,
                (unsigned short) client->received_data_size
        );

        if (client->rsa_decrypted) return packet_blowfish_decrypted;
        
        decrypted_packet = login_client_alloc_temp_mem(
                client,
                MAX_DATA_FROM_PACKET
        );

        login_client_decrypt_rsa(
                client,
                packet_blowfish_decrypted,
                decrypted_packet
        );

        client->rsa_decrypted = 1;

        return decrypted_packet;
}

void* login_client_alloc_temp_mem(struct LoginClient* client, size_t how_much)
{
        assert(client);
        assert(how_much);
        assert(how_much <= TEMP_MEM_PER_CLIENT);
        return (void *) circular_memory_alloc(client->temp_memory, how_much);
}

byte_builder* login_client_byte_builder
(
        struct LoginClient* client,
        size_t how_much
)
{
        assert(client);
        assert(how_much);

        size_t to_be_allocated = byte_builder_calculate_size(how_much);
        byte_builder* builder = login_client_alloc_temp_mem(
                client,
                to_be_allocated
        );

        return byte_builder_init(builder, to_be_allocated);
}

int login_client_connection_ended(struct LoginClient* client)
{
        assert(client);
        return client->received_data_size <= 0;
}

l2_raw_packet* login_client_create_raw_packet
(
        struct LoginClient* client,
        unsigned char* content,
        unsigned short content_size
)
{
        assert(client);
        assert(content);
        assert(content_size);

        l2_raw_packet_size packet_size =
                l2_raw_packet_calculate_size(content_size);
        l2_raw_packet* packet =
                login_client_alloc_temp_mem(client, packet_size);

        l2_raw_packet_init(packet, content, content_size);

        return packet;
}

l2_packet* login_client_create_packet
(
        struct LoginClient* client,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
)
{
        assert(client);
        assert(content);
        assert(content_size);

        l2_raw_packet_size packet_size =
                l2_packet_calculate_size(content_size);
        l2_packet* packet =
                login_client_alloc_temp_mem(client, packet_size);

        l2_packet_init(packet, type, content, content_size);

        return packet;
}

size_t login_client_struct_size()
{
        return sizeof(struct LoginClient);
}
