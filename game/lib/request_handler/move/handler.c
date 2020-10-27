#include <assert.h>
#include <log/log.h>
#include <core/byte_reader.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>
#include "../../request.h"
#include "../../client.h"
#include "../../dto/player.h"
#include "../../dto/vec3.h"
#include "../npc_info/handler.h"
#include "../../world_manager.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void move_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        l2_raw_packet *packet = request->packet;

        unsigned char *content = NULL;
        struct Player *player = NULL;

        struct Vec3 prev_location;
        struct Vec3 new_location;

        content = l2_packet_content(packet);
        player = client_player(client);

        prev_location.x = player->character.x;
        prev_location.y = player->character.y;
        prev_location.z = player->character.z;

        content = byte_reader_cpy_int_n_mv(content, &new_location.x);
        content = byte_reader_cpy_int_n_mv(content, &new_location.y);
        content = byte_reader_cpy_int_n_mv(content, &new_location.z);

        log_info(
            "Trying to move character from x: %d, y: %d, z: %d to x: %d, y: %d, z: %d",
            prev_location.x,
            prev_location.y,
            prev_location.z,
            new_location.x,
            new_location.y,
            new_location.z);

        world_manager_move_active_char(request->world_state, client, &prev_location, &new_location);

        client_free_mem(client, player);
}
