#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <log/log.h>
#include <core/memory.h>
#include <core/circular_memory_alloc.h>
#include <core/l2_rsa_key.h>
#include <core/l2_blowfish_key.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <os/socket.h>
#include <login/dto/session_key.h>
#include <login/packet/blowfish_encrypt.h>
#include <login/packet/blowfish_decrypt.h>
#include <game/dto/char.h>
#include <core/l2_client.h>

#define TEMP_MEMORY_PER_CLIENT_IN_BYTES 131072
#define MEMORY_PER_CLIENT_IN_BYTES 131072

struct L2Client
{
        os_socket_handler* socket_handler;
        struct L2RSAKey* rsa_key;
        struct L2BlowfishKey* blowfish_key;

        struct LoginDtoSessionKey session;

        size_t received_data_size;
        unsigned char received_data[L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES];

        circular_memory_space temp_memory[TEMP_MEMORY_PER_CLIENT_IN_BYTES];
        memory preallocated_memory[MEMORY_PER_CLIENT_IN_BYTES];

        struct GameDtoChar character;
};

struct LoginDtoSessionKey* l2_client_session(struct L2Client* client)
{
        assert(client);
        return &client->session;
}

struct L2RSAKey* l2_client_rsa_key(struct L2Client* client)
{
        assert(client);
        return client->rsa_key;
}

struct L2BlowfishKey* l2_client_blowfish_key(struct L2Client* client)
{
        assert(client);
        return client->blowfish_key;
}

size_t l2_client_struct_size()
{
        return sizeof(struct L2Client);
}

struct GameDtoChar* l2_client_get_char(struct L2Client* client)
{
        return &client->character;
}

void l2_client_init(struct L2Client* client)
{
        assert(client);

        client->received_data_size = 0;

        login_session_key_init(&client->session);

        memory_init(
                client->preallocated_memory,
                MEMORY_PER_CLIENT_IN_BYTES
        );

        circular_memory_alloc_init(
                client->temp_memory,
                TEMP_MEMORY_PER_CLIENT_IN_BYTES
        );

        client->rsa_key = l2_client_alloc(
                client,
                l2_rsa_key_struct_size()
        );
        
        client->blowfish_key = l2_client_alloc(
                client,
                l2_blowfish_key_struct_size()
        );

        client->socket_handler = l2_client_alloc(
                client,
                os_socket_handler_size()
        );

        l2_rsa_key_init(client->rsa_key);
        l2_blowfish_key_init(client->blowfish_key);
}

byte_builder* l2_client_byte_builder
(
        struct L2Client* client,
        size_t how_much
)
{
        assert(client);
        assert(how_much);

        size_t to_be_allocated = byte_builder_calculate_size(how_much);
        byte_builder* builder = l2_client_alloc_temp_mem(
                client,
                to_be_allocated
        );

        return byte_builder_init(builder, to_be_allocated);
}

void* l2_client_alloc(struct L2Client* client, size_t how_much)
{
        assert(client);
        assert(how_much);
        return memory_alloc(client->preallocated_memory, how_much);
}

void l2_client_alloc_free(struct L2Client* client, void* block)
{
        assert(client);
        assert(block);
        memory_free(block);
}

void* l2_client_alloc_temp_mem(struct L2Client* client, size_t how_much)
{
        assert(client);
        assert(how_much);
        return (void *) circular_memory_alloc(client->temp_memory, how_much);
}

l2_raw_packet* l2_client_create_raw_packet
(
        struct L2Client* client,
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
                l2_client_alloc_temp_mem(client, packet_size);

        l2_raw_packet_init(packet, content, content_size);

        return packet;
}

l2_packet* l2_client_create_packet
(
        struct L2Client* client,
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
                l2_client_alloc_temp_mem(client, packet_size);

        l2_packet_init(packet, type, content, content_size);

        return packet;
}

void l2_client_accept
(
        struct L2Client* client,
        os_socket_handler* server_socket
)
{
        assert(client);
        assert(server_socket);
        os_socket_accept(server_socket, client->socket_handler);
        log_info("Connection accepted");
}

void l2_client_close(struct L2Client* client)
{
        assert(client);

        l2_client_alloc_free(client, client->rsa_key);
        l2_client_alloc_free(client, client->blowfish_key);

        os_socket_close(client->socket_handler);
        l2_client_alloc_free(client, client->socket_handler);

        log_info("Connection closed");
}

void l2_client_send_packet(struct L2Client* client, l2_raw_packet* packet)
{
        assert(client);
        assert(packet);

        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        char *printable = l2_client_alloc_temp_mem(
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

void l2_client_encrypt_and_send_packet
(
        struct L2Client* client,
        l2_raw_packet* packet
)
{
        assert(client);
        assert(packet);

        l2_raw_packet *encrypted_packet =
                login_packet_blowfish_encrypt(client, packet);

        l2_client_send_packet(client, encrypted_packet);
}

l2_raw_packet* l2_client_wait_packet(struct L2Client* client)
{
        assert(client);

        unsigned char* content_without_size_header = NULL;

        client->received_data_size = os_socket_receive(
                client->socket_handler,
                client->received_data,
                L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES
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

        l2_raw_packet* packet = l2_client_alloc_temp_mem(
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

l2_raw_packet* l2_client_wait_and_decrypt_packet(struct L2Client* client)
{
        assert(client);

        client->received_data_size = os_socket_receive(
                client->socket_handler,
                client->received_data,
                L2_CLIENT_MAX_DATA_TO_RECEIVE_IN_BYTES
        );

        if (client->received_data_size == 0) {
                return NULL;
        }

        return login_packet_blowfish_decrypt(
                client,
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
        assert(client);
        assert(packet);
        assert(dest);

        unsigned short packet_content_size =
                l2_packet_get_content_size(packet);
        unsigned char* packet_content = l2_client_alloc_temp_mem(
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

int l2_client_connection_ended(struct L2Client* client)
{
        assert(client);
        return client->received_data_size <= 0;
}
