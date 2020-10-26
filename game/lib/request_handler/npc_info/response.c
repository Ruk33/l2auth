#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/byte_builder.h>
#include <core/l2_packet.h>
#include <core/l2_string.h>
#include "../../dto/npc.h"
#include "../../client.h"
#include "../../packet_builder.h"
#include "handler.h"

l2_packet *npc_info_response(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x16;
        l2_packet *response = NULL;

        struct Npc npc;

        int attackable = 1;

        int empty = 0x00;
        double empty_d = 0;
        char empty_c = 0;

        char name_above_char = 1;
        char running = 0;
        char combat = 0;
        char alike_dead = 0;
        char summonned = 0;
        double f_1 = 1.1;

        size_t name_len = 0;
        size_t name_as_string_length = 0;
        l2_string *name_as_string = NULL;

        size_t title_len = 0;
        size_t title_as_string_length = 0;
        l2_string *title_as_string = NULL;

        size_t buf_size = byte_builder_calculate_size(1024);
        byte_builder *buf = client_alloc_mem(client, buf_size);
        byte_builder *buffer = byte_builder_init(buf, buf_size);

        npc.character.id = 1;
        npc.template_id = 7006 + 1000000;

        // Roxxy -84108,244604,-3654, 40960
        npc.character.x = -84108;
        npc.character.y = 244604;
        npc.character.z = -3654;
        npc.character.heading = 40960;

        npc.collision_radius = 8.0;
        npc.collision_height = 25.0;

        memset(npc.character.title, 0, sizeof(npc.character.title));
        memset(npc.character.name, 0, sizeof(npc.character.name));

        strcat(npc.character.title, "Gatekeeper");
        strcat(npc.character.name, "Roxxy");

        name_len = strlen(npc.character.name) + 1;
        title_len = strlen(npc.character.title) + 1;

        name_as_string_length = l2_string_calculate_space_from_char(name_len);
        name_as_string = client_alloc_mem(client, name_as_string_length);

        title_as_string_length = l2_string_calculate_space_from_char(title_len);
        title_as_string = client_alloc_mem(client, title_as_string_length);

        l2_string_from_char(name_as_string, npc.character.name, name_len);
        l2_string_from_char(title_as_string, npc.character.title, title_len);

        byte_builder_append_int(buffer, &npc.character.id);
        byte_builder_append_int(buffer, &npc.template_id);
        byte_builder_append_int(buffer, &attackable);
        byte_builder_append_int(buffer, &npc.character.x);
        byte_builder_append_int(buffer, &npc.character.y);
        byte_builder_append_int(buffer, &npc.character.z);
        byte_builder_append_int(buffer, &npc.character.heading);

        byte_builder_append_int(buffer, &empty);

        // stats
        byte_builder_append_int(buffer, &empty); // mattack speed
        byte_builder_append_int(buffer, &empty); // pattack speed
        byte_builder_append_int(buffer, &empty); // run speed
        byte_builder_append_int(buffer, &empty); // walk speed
        byte_builder_append_int(buffer, &empty); // swim run speed
        byte_builder_append_int(buffer, &empty); // swim w speed
        byte_builder_append_int(buffer, &empty); // fl run speed
        byte_builder_append_int(buffer, &empty); // fl w speed
        byte_builder_append_int(buffer, &empty); // fly run speed
        byte_builder_append_int(buffer, &empty); // fly w speed

        byte_builder_append_double(buffer, &f_1);

        byte_builder_append_double(buffer, &empty_d);              // p atk speed
        byte_builder_append_double(buffer, &npc.collision_radius); // col radius
        byte_builder_append_double(buffer, &npc.collision_height); // col height

        byte_builder_append_int(buffer, &empty); // r weapon
        byte_builder_append_int(buffer, &empty);

        byte_builder_append_int(buffer, &empty); // l weapon

        byte_builder_append_char(buffer, &name_above_char);
        byte_builder_append_char(buffer, &running);
        byte_builder_append_char(buffer, &combat);
        byte_builder_append_char(buffer, &alike_dead);
        byte_builder_append_char(buffer, &summonned);

        // name & title
        byte_builder_append(buffer, name_as_string, name_as_string_length);
        byte_builder_append(buffer, title_as_string, title_as_string_length);

        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);

        byte_builder_append_int(buffer, &empty); // ab effects
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_char(buffer, &empty_c);

        byte_builder_append_char(buffer, &empty_c);
        byte_builder_append_double(buffer, &empty_d);
        byte_builder_append_double(buffer, &empty_d);
        byte_builder_append_int(buffer, &empty);

        // client_spawn_npc(client, &npc);

        response = packet_builder_new(
            client,
            type,
            buffer,
            (unsigned short)byte_builder_length(buffer));

        client_free_mem(client, name_as_string);
        client_free_mem(client, title_as_string);
        client_free_mem(client, buf);

        return response;
}
