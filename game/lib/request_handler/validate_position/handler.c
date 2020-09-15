#include <assert.h>
#include <core/l2_raw_packet.h>
#include <core/byte_reader.h>
#include "../../client.h"
#include "../../dto/pc.h"
#include "../../dto/vec3.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void validate_position_handler
(struct Client *client, l2_raw_packet *packet)
{
        assert(client);
        assert(packet);

        // l2_packet *response = NULL;
        // unsigned char* content = l2_packet_content(packet);
        // assert(content);

        // struct Pc *player = client_player(client);
        // assert(player);

        // int heading = 0;
        // struct Vec3 location;
        // struct Vec3 current_location;

        // current_location.x = player->character.x;
        // current_location.y = player->character.y;
        // current_location.z = player->character.z;

        // content = byte_reader_cpy_int_n_mv(content, &location.x);
        // content = byte_reader_cpy_int_n_mv(content, &location.y);
        // content = byte_reader_cpy_int_n_mv(content, &location.z);
        // content = byte_reader_cpy_int_n_mv(content, &heading);

        // response = validate_position_response(client, location, heading);
        // assert(response);

        // client_encrypt_packet(client, response);
        // client_queue_response(client, response);

        client_update_request_handler(client, &validate_position_next_handler);

        // client_free_mem(client, response);
        // client_free_mem(client, player);
}
