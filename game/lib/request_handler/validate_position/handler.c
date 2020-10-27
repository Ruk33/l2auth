#include <assert.h>
#include <core/l2_raw_packet.h>
#include <core/byte_reader.h>
#include "../../request.h"
#include "../../client.h"
#include "../../dto/player.h"
#include "../../dto/vec3.h"
#include "../../entity/player.h"
#include "response.h"
#include "next_handler.h"
#include "handler.h"

void validate_position_handler(struct Request *request)
{
        assert(request);

        struct Client *client = request->client;
        unsigned char *content = l2_packet_content(request->packet);
        int heading = 0;
        struct Vec3 location;

        content = byte_reader_cpy_int_n_mv(content, &location.x);
        content = byte_reader_cpy_int_n_mv(content, &location.y);
        content = byte_reader_cpy_int_n_mv(content, &location.z);
        content = byte_reader_cpy_int_n_mv(content, &heading);

        player_validate_location(client, &location, heading);
}
