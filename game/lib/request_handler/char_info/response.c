#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/l2_packet.h>
#include <core/l2_string.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "../../dto/character.h"
#include "../../dto/player.h"
#include "response.h"

l2_packet *char_info_response(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x03;

        struct Player *player = client_player(client);
        struct Character *character = &player->character;
        size_t name_as_string_length = l2_string_calculate_space_from_char(strlen(character->name) + 1);
        l2_string *name_as_string = client_alloc_mem(client, name_as_string_length);
        int empty = 0;
        int pvp_flag = 0;
        int karma = 0;
        int swim_run_speed = 10;
        int swim_walk_speed = 10;
        int fly_run_speed = 10;
        int fly_walk_speed = 10;
        double collision_radius = 9;
        double collision_height = 23;
        char title[] = {0, 0};
        int clan_crest_id = 0;
        int ally_crest_id = 0;
        char standing = 1;
        char running = 1;
        char in_combat = 0;
        char alike_dead = 0;
        char invisible = 0;
        char mount_type = 0;
        char private_store_type = 0;
        short cubics = 0;
        char find_party_member = 0;
        int abnormal_effect = 0;
        char empty_char = 0;
        char mounted = 0;
        int clan_crest_large_id = 0;
        char hero_symbol = 0;
        char hero_aura = 0;
        char fishing = 0;
        int name_color = 0xffffff;

        l2_packet *response = NULL;
        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *buffer = NULL;
        int run_speed = 120;
        int walk_speed = 100;
        double mov_mult = 1;

        buf_size = byte_builder_calculate_size(1024);
        buf = client_alloc_mem(client, buf_size);
        buffer = byte_builder_init(buf, buf_size);

        memset(name_as_string, 0, name_as_string_length);
        l2_string_from_char(name_as_string, character->name, strlen(character->name));

        byte_builder_append_int(buffer, &character->x);
        byte_builder_append_int(buffer, &character->y);
        byte_builder_append_int(buffer, &character->z);
        byte_builder_append_int(buffer, &character->heading);
        byte_builder_append_int(buffer, &character->id);

        byte_builder_append(buffer, name_as_string, name_as_string_length);

        byte_builder_append_int(buffer, &player->race_id);
        byte_builder_append_int(buffer, &character->sex);

        byte_builder_append_int(buffer, &player->class_id);
        byte_builder_append_int(buffer, &empty);

        // paperdoll
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);

        byte_builder_append_int(buffer, &pvp_flag);
        byte_builder_append_int(buffer, &player->karma);

        byte_builder_append_int(buffer, &character->m_attack_speed);
        byte_builder_append_int(buffer, &character->attack_speed);

        byte_builder_append_int(buffer, &pvp_flag);
        byte_builder_append_int(buffer, &karma);

        byte_builder_append_int(buffer, &run_speed);
        byte_builder_append_int(buffer, &walk_speed);
        byte_builder_append_int(buffer, &swim_run_speed);
        byte_builder_append_int(buffer, &swim_walk_speed);
        byte_builder_append_int(buffer, &fly_run_speed);
        byte_builder_append_int(buffer, &fly_walk_speed);
        byte_builder_append_int(buffer, &fly_run_speed);
        byte_builder_append_int(buffer, &fly_walk_speed);

        byte_builder_append_double(buffer, &mov_mult);
        byte_builder_append_double(buffer, (double *)&player->attack_speed_multiplier);
        byte_builder_append_double(buffer, &collision_radius);
        byte_builder_append_double(buffer, &collision_height);

        byte_builder_append_int(buffer, &player->hair_style_id);
        byte_builder_append_int(buffer, &player->hair_color_id);
        byte_builder_append_int(buffer, &player->face);

        byte_builder_append(buffer, title, sizeof(title));

        byte_builder_append_int(buffer, &player->clan_id);
        byte_builder_append_int(buffer, &clan_crest_id);
        byte_builder_append_int(buffer, &player->ally_id);
        byte_builder_append_int(buffer, &ally_crest_id);
        byte_builder_append_int(buffer, &empty);

        byte_builder_append_char(buffer, &standing);
        byte_builder_append_char(buffer, &running);
        byte_builder_append_char(buffer, &in_combat);
        byte_builder_append_char(buffer, &alike_dead);

        byte_builder_append_char(buffer, &invisible);
        byte_builder_append_char(buffer, &mount_type);
        byte_builder_append_char(buffer, &private_store_type);

        byte_builder_append_short(buffer, &cubics);

        byte_builder_append_char(buffer, &find_party_member);

        byte_builder_append_int(buffer, &abnormal_effect);

        byte_builder_append_char(buffer, &empty_char);
        byte_builder_append_short(buffer, (short *)&player->recommendations_left);
        byte_builder_append_int(buffer, &empty);

        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_char(buffer, &mounted);

        byte_builder_append_char(buffer, &empty_char);

        byte_builder_append_int(buffer, &clan_crest_large_id);
        byte_builder_append_char(buffer, &hero_symbol);
        byte_builder_append_char(buffer, &hero_aura);

        byte_builder_append_char(buffer, &fishing);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);
        byte_builder_append_int(buffer, &empty);

        byte_builder_append_int(buffer, &name_color);

        response = packet_builder_new(
            client,
            type,
            buffer,
            byte_builder_length(buffer));

        client_free_mem(client, buf);
        client_free_mem(client, name_as_string);

        return response;
}
