#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <game/server.h>
#include <game/client.h>
#include "handler.h"

void game_request_action_handler
(
        struct GameServer* server,
        struct GameClient* client,
        l2_raw_packet* request,
        unsigned char* encrypt_key
)
{
        assert(server);
        assert(client);
        assert(request);
        assert(encrypt_key);
        unsigned char* request_content = l2_packet_content(request);

        int object_id = 0;
        int origin_x = 0;
        int origin_y = 0;
        int origin_z = 0;
        char action_id = 0;

        request_content = byte_reader_copy_and_move(
                request_content,
                &object_id,
                sizeof(object_id)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &origin_x,
                sizeof(origin_x)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &origin_y,
                sizeof(origin_y)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &origin_z,
                sizeof(origin_z)
        );

        request_content = byte_reader_copy_and_move(
                request_content,
                &action_id,
                sizeof(action_id)
        );

        log_info("Action request handler");
        log_info("Object id: %d", object_id);
        log_info("Origin x: %d", origin_x);
        log_info("Origin y: %d", origin_y);
        log_info("Origin z: %d", origin_z);
        log_info("Action id: %d", action_id);
}
