#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <core/l2_string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <game/client.h>
#include <game/entity/player.h>
#include "response.h"

l2_packet* game_request_character_info_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0x03;
        struct Player* player = game_client_get_char(client);
        size_t name_as_string_length = l2_string_calculate_space_from_char(strlen(player->character.name) + 1);
        l2_string* name_as_string = game_client_alloc_temp_mem(client, name_as_string_length);
        int heading = 0;
        int empty = 0;
        int pvp_flag = 0;
        int karma = 0;
        int run_speed = 120;
        int walk_speed = 100;
        int swim_run_speed = 10;
        int swim_walk_speed = 10;
        int fly_run_speed = 10;
        int fly_walk_speed = 10;
        double movement_speed_multiplier = 1;
        double atk_speed_multiplier = 1;
        double collision_radius = 9;
        double collision_height = 23;
        char title[] = { 0, 0 };
        int clan_id = 0;
        int clan_crest_id = 0;
        int ally_id = 0;
        int ally_crest_id = 0;
        char sitting[] = { 0 };
        char running[] = { 1 };
        char in_combat[] = { 0 };
        char alike_dead[] = { 0 };
        char invisible[] = { 0 };
        char mount_type[] = { 0 };
        char private_store_type[] = { 0 };
        short cubics = 0;
        char find_party_member[] = { 0 };
        int abnormal_effect = 0;
        char empty_char[] = { 0 };
        short recom_have = 0;
        char mounted[] = { 0 };
        int clan_crest_large_id = 0;
        char hero_symbol[] = { 0 };
        char hero_aura[] = { 0 };
        char fishing[] = { 0 };
        int name_color = 0xffffff;
        int m_attack_speed = 1;
        int p_attack_speed = 1;
        
        byte_builder* buffer = game_client_byte_builder(client, 1000);
        assert(buffer);

        memset(name_as_string, 0, name_as_string_length);
        l2_string_from_char(name_as_string, player->character.name, strlen(player->character.name));

        byte_builder_append(buffer, &player->character.x, sizeof(player->character.x));
        byte_builder_append(buffer, &player->character.y, sizeof(player->character.y));
        byte_builder_append(buffer, &player->character.z, sizeof(player->character.z));
        byte_builder_append(buffer, &heading, sizeof(heading));
        byte_builder_append(buffer, &player->character.id, sizeof(player->character.id));
        byte_builder_append(buffer, name_as_string, name_as_string_length);
        byte_builder_append(buffer, &player->race_id, sizeof(player->race_id));
        byte_builder_append(buffer, &player->character.sex, sizeof(player->character.sex));
        byte_builder_append(buffer, &player->class_id, sizeof(player->class_id));
        byte_builder_append(buffer, &empty, sizeof(empty));

        // paperdoll
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, &pvp_flag, sizeof(pvp_flag));
        byte_builder_append(buffer, &karma, sizeof(karma));

        byte_builder_append(buffer, &m_attack_speed, sizeof(m_attack_speed));
        byte_builder_append(buffer, &p_attack_speed, sizeof(p_attack_speed));

        byte_builder_append(buffer, &pvp_flag, sizeof(pvp_flag));
        byte_builder_append(buffer, &karma, sizeof(karma));

        byte_builder_append(buffer, &run_speed, sizeof(run_speed));
        byte_builder_append(buffer, &walk_speed, sizeof(walk_speed));
        byte_builder_append(buffer, &swim_run_speed, sizeof(swim_run_speed));
        byte_builder_append(buffer, &swim_walk_speed, sizeof(swim_walk_speed));
        byte_builder_append(buffer, &fly_run_speed, sizeof(fly_run_speed));
        byte_builder_append(buffer, &fly_walk_speed, sizeof(fly_walk_speed));
        byte_builder_append(buffer, &fly_run_speed, sizeof(fly_run_speed));
        byte_builder_append(buffer, &fly_walk_speed, sizeof(fly_walk_speed));

        byte_builder_append(buffer, &movement_speed_multiplier, sizeof(movement_speed_multiplier));
        byte_builder_append(buffer, &atk_speed_multiplier, sizeof(atk_speed_multiplier));
        byte_builder_append(buffer, &collision_radius, sizeof(collision_radius));
        byte_builder_append(buffer, &collision_height, sizeof(collision_height));

        byte_builder_append(buffer, &player->hair_style_id, sizeof(player->hair_style_id));
        byte_builder_append(buffer, &player->hair_color_id, sizeof(player->hair_color_id));
        byte_builder_append(buffer, &player->face, sizeof(player->face));

        byte_builder_append(buffer, title, sizeof(title));
        byte_builder_append(buffer, &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, &clan_crest_id, sizeof(clan_crest_id));
        byte_builder_append(buffer, &ally_id, sizeof(ally_id));
        byte_builder_append(buffer, &ally_crest_id, sizeof(ally_crest_id));
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, sitting, sizeof(sitting));
        byte_builder_append(buffer, running, sizeof(running));
        byte_builder_append(buffer, in_combat, sizeof(in_combat));
        byte_builder_append(buffer, alike_dead, sizeof(alike_dead));

        byte_builder_append(buffer, invisible, sizeof(invisible));
        byte_builder_append(buffer, mount_type, sizeof(mount_type));
        byte_builder_append(buffer, private_store_type, sizeof(private_store_type));

        byte_builder_append(buffer, &cubics, sizeof(cubics));

        byte_builder_append(buffer, find_party_member, sizeof(find_party_member));

        byte_builder_append(buffer, &abnormal_effect, sizeof(abnormal_effect));

        byte_builder_append(buffer, empty_char, sizeof(empty_char));
        byte_builder_append(buffer, &recom_have, sizeof(recom_have));
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, mounted, sizeof(mounted));

        byte_builder_append(buffer, empty_char, sizeof(empty_char));

        byte_builder_append(buffer, &clan_crest_large_id, sizeof(clan_crest_large_id));
        byte_builder_append(buffer, hero_symbol, sizeof(hero_symbol));
        byte_builder_append(buffer, hero_aura, sizeof(hero_aura));

        byte_builder_append(buffer, fishing, sizeof(fishing));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, &name_color, sizeof(name_color));

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
