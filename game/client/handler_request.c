#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <os/memory.h>
#include <core/l2_packet.h>
#include <core/l2_raw_packet.h>
#include <game/state.h>
#include <game/client_connection.h>
#include <game/connection.h>
#include <game/request.h>
#include <game/service/crypt/decrypt.h>

void game_client_handler_request
(
        struct GameState* game_state,
        struct GameClientConnection* client,
        void* request,
        size_t request_size
)
{
        assert(game_state);
        assert(client);
        assert(client->conn);
        assert(client->memory);
        assert(request);

        struct GameConnection* game_connection = (struct GameConnection*) client->conn;
        struct GameRequest* game_request = memory_alloc(
                client->memory,
                sizeof(struct GameRequest)
        );

        unsigned short packet_size = 0;
        l2_packet_type packet_type;

        unsigned char* content_without_size_header = request + sizeof(l2_raw_packet_size);
        unsigned short content_without_size_header_size = (unsigned short) (request_size - sizeof(short));
        l2_raw_packet* packet = memory_alloc(
                client->memory,
                l2_raw_packet_calculate_size(content_without_size_header_size)
        );

        l2_raw_packet_init(
                packet,
                content_without_size_header,
                (unsigned short) content_without_size_header_size
        );

        memcpy(&packet_size, packet, sizeof(packet_size));
        if (packet_size > 1) packet_size = (unsigned short) (packet_size - 2);

        if (game_connection->is_encrypted) {
                game_service_crypt_decrypt(
                        packet + 2,
                        packet_size,
                        game_connection->decrypt_key
                );
        }

        packet_type = (l2_packet_type) (packet[2] & 0xff);

        log_info("Packet type %02X", packet_type);

        game_request->conn = game_connection;
        game_request->packet = packet;

        assert(game_connection->handler);
        game_connection->handler(game_request);

        memory_free(packet);
        memory_free(game_request);
}
