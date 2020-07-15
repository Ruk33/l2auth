#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <game/client.h>
#include "response.h"

l2_packet* game_request_npc_info_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0x16;

        int obj_id = 1;
        int template_id = 7006 + 1000000;
        int attackable = 1;

        /// Validating position - x: -84760, y: 242436, z: -3730
        // -84594, y: 242387, z: -3730
        // roxxy -84108,244604,-3654, 40960
        int x = -84108;
        int y = 244604;
        int z = -3654;
        int heading = 40960;

        int empty = 0x00;
        double empty_d = 0;
        char empty_c = 0;

        char name_above_char = 1;
        char running = 0;
        char combat = 0;
        char alike_dead = 0;
        char summonned = 0;
        double f_1 = 1.1;

        double col_radius = 8.0;
        double col_height = 25.0;

        char* name = game_client_alloc_temp_mem(client, 20);
        assert(name);

        char* title = game_client_alloc_temp_mem(client, 20);
        assert(title);

        size_t name_as_string_length;
        l2_string* name_as_string;

        size_t title_as_string_length;
        l2_string* title_as_string;

        strcat(title, "Gatekeeper");
        strcat(name, "Roxxy");

        name_as_string_length = l2_string_calculate_space_from_char(strlen(name) + 1);
        name_as_string = game_client_alloc_temp_mem(client, name_as_string_length);
        assert(name_as_string);

        title_as_string_length = l2_string_calculate_space_from_char(strlen(title) + 1);
        title_as_string = game_client_alloc_temp_mem(client, title_as_string_length);
        assert(title_as_string);

        l2_string_from_char(name_as_string, name, strlen(name));
        l2_string_from_char(title_as_string, title, strlen(title));

        byte_builder* buffer = game_client_byte_builder(client, 1000);
        assert(buffer);

        byte_builder_append(buffer, &obj_id, sizeof(obj_id));
        byte_builder_append(buffer, &template_id, sizeof(template_id));
        byte_builder_append(buffer, &attackable, sizeof(attackable));
        byte_builder_append(buffer, &x, sizeof(x));
        byte_builder_append(buffer, &y, sizeof(y));
        byte_builder_append(buffer, &z, sizeof(z));
        byte_builder_append(buffer, &heading, sizeof(heading));

        byte_builder_append(buffer, &empty, sizeof(empty));

        // stats
        byte_builder_append(buffer, &empty, sizeof(empty)); // mattack speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // pattack speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // run speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // walk speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // swim run speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // swim w speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // fl run speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // fl w speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // fly run speed
        byte_builder_append(buffer, &empty, sizeof(empty)); // fly w speed

        byte_builder_append(buffer, &f_1, sizeof(f_1));

        byte_builder_append(buffer, &empty_d, sizeof(empty_d)); // p atk speed
        byte_builder_append(buffer, &col_radius, sizeof(col_radius)); // col radius
        byte_builder_append(buffer, &col_height, sizeof(col_height)); // col height

        byte_builder_append(buffer, &empty, sizeof(empty)); // r weapon
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, &empty, sizeof(empty)); // l weapon

        byte_builder_append(buffer, &name_above_char, sizeof(name_above_char));
        byte_builder_append(buffer, &running, sizeof(running));
        byte_builder_append(buffer, &combat, sizeof(combat));
        byte_builder_append(buffer, &alike_dead, sizeof(alike_dead));
        byte_builder_append(buffer, &summonned, sizeof(summonned));

        // name & title
        byte_builder_append(buffer, name_as_string, name_as_string_length);
        byte_builder_append(buffer, title_as_string, title_as_string_length);

        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, &empty, sizeof(empty)); // ab effects
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty_c, sizeof(empty_c));

        byte_builder_append(buffer, &empty_c, sizeof(empty_c));
        byte_builder_append(buffer, &empty_d, sizeof(empty_d));
        byte_builder_append(buffer, &empty_d, sizeof(empty_d));
        byte_builder_append(buffer, &empty, sizeof(empty));

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
