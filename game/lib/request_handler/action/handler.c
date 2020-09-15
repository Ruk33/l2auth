#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../client.h"
#include "../enter_world/next_handler.h"
#include "../../entity/character.h"
#include "../../entity/pc.h"
#include "handler.h"

void action_handler
(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        unsigned char *content = l2_packet_content(packet);
        struct Character *target = NULL;

        int object_id = 0;
        int origin_x = 0;
        int origin_y = 0;
        int origin_z = 0;
        int action_id = 0;

        content = byte_reader_cpy_int_n_mv(content, &object_id);
        content = byte_reader_cpy_int_n_mv(content, &origin_x);
        content = byte_reader_cpy_int_n_mv(content, &origin_y);
        content = byte_reader_cpy_int_n_mv(content, &origin_z);
        content = byte_reader_cpy_int_n_mv(content, &action_id);

        log_info("Action request handler");
        log_info("Object id: %d", object_id);
        log_info("Origin x: %d", origin_x);
        log_info("Origin y: %d", origin_y);
        log_info("Origin z: %d", origin_z);
        log_info("Action id: %d", action_id);

        target = client_character(client, object_id);

        player_entity_action(client, target);
        client_update_request_handler(client, &enter_world_next_handler);

        client_free_mem(client, target);
}
