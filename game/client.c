#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <log/log.h>
#include <core/memory.h>
#include <core/circular_memory_alloc.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/session_key.h>
#include <os/socket.h>
#include <game/entity/character.h>
#include "client.h"

size_t game_client_struct_size()
{
        return sizeof(struct GameClient);
}

struct L2SessionKey* game_client_session(struct GameClient* client)
{
        assert(client);
        return &client->session;
}

struct GameEntityCharacter* game_client_get_char(struct GameClient* client)
{
        return &client->character;
}

void game_client_init(struct GameClient* client)
{
        assert(client);

        client->received_data_size = 0;

        l2_session_key_init(&client->session);

        memory_init(
                client->preallocated_memory,
                MEMORY_PER_CLIENT_IN_BYTES
        );

        circular_memory_alloc_init(
                client->temp_memory,
                TEMP_MEMORY_PER_CLIENT_IN_BYTES
        );

        // client->socket_handler = game_client_alloc(
        //         client,
        //         os_socket_handler_size()
        // );
}

byte_builder* game_client_byte_builder
(
        struct GameClient* client,
        size_t how_much
)
{
        assert(client);
        assert(how_much);

        size_t to_be_allocated = byte_builder_calculate_size(how_much);
        byte_builder* builder = game_client_alloc_temp_mem(
                client,
                to_be_allocated
        );

        return byte_builder_init(builder, to_be_allocated);
}

void* game_client_alloc(struct GameClient* client, size_t how_much)
{
        assert(client);
        assert(how_much);
        return memory_alloc(client->preallocated_memory, how_much);
}

void game_client_alloc_free(struct GameClient* client, void* block)
{
        assert(client);
        assert(block);
        memory_free(block);
}

void* game_client_alloc_temp_mem(struct GameClient* client, size_t how_much)
{
        assert(client);
        assert(how_much);
        return (void *) circular_memory_alloc(client->temp_memory, how_much);
}

l2_raw_packet* game_client_create_raw_packet
(
        struct GameClient* client,
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
                game_client_alloc_temp_mem(client, packet_size);

        l2_raw_packet_init(packet, content, content_size);

        return packet;
}

l2_packet* game_client_create_packet
(
        struct GameClient* client,
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
                game_client_alloc_temp_mem(client, packet_size);

        l2_packet_init(packet, type, content, content_size);

        return packet;
}

void game_client_accept
(
        struct GameClient* client,
        os_socket_handler* server_socket
)
{
        assert(client);
        assert(server_socket);
        os_socket_accept(server_socket, client->socket_handler);
        log_info("Connection accepted");
}

void game_client_close(struct GameClient* client)
{
        assert(client);

        os_socket_close(client->socket_handler);
        game_client_alloc_free(client, client->socket_handler);

        log_info("Connection closed");
}

void game_client_send_packet(struct GameClient* client, l2_raw_packet* packet)
{
        assert(client);
        assert(packet);

        l2_raw_packet_size packet_size = l2_raw_packet_get_size(packet);
        char *printable = game_client_alloc_temp_mem(
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

l2_raw_packet* game_client_wait_packet(struct GameClient* client)
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

        l2_raw_packet* packet = game_client_alloc_temp_mem(
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

int game_client_connection_ended(struct GameClient* client)
{
        assert(client);
        return client->received_data_size <= 0;
}
