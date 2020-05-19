#include <assert.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <game/connection.h>
#include <game/server.h>
#include <game/client.h>
#include <game/service/crypt/decrypt.h>
#include <game/request.h>
#include <game/request/type.h>
#include "handler.h"

int game_request_entry_point_handler(struct GameConnection* conn)
{
        assert(conn);

        struct GameRequest request;

        struct GameServer* server = conn->server;
        struct GameClient* client = conn->client;

        l2_raw_packet *client_packet = game_client_wait_packet(client);

        unsigned short packet_size = 0;
        l2_packet_type packet_type;

        if (game_client_connection_ended(client)) return 1;

        memcpy(&packet_size, client_packet, sizeof(packet_size));
        if (packet_size > 1) {
                packet_size = (unsigned short) (packet_size - 2);
        }

        if (conn->is_encrypted) {
                game_service_crypt_decrypt(
                        client_packet + 2,
                        packet_size,
                        conn->decrypt_key
                );
        }

        /*
         * (franco.montenegro)
         * Check why l2_packet_get_type is not working here
         */
        packet_type = (l2_packet_type) (client_packet[2] & 0xff);

        log_error("Packet type %02X", packet_type);

        request.conn = conn;
        request.packet = client_packet;

        conn->handler(&request);

        return 0;
}
