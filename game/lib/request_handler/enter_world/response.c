#include <assert.h>
#include <string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/l2_string.h>
#include "../../client.h"
#include "../../entity/player.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *enter_world_response
(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x04;
        l2_packet *response = NULL;

        size_t buf_size = byte_builder_calculate_size(1024);
        byte_builder *buf = client_alloc_mem(client, buf_size);
        byte_builder *buffer = byte_builder_init(buf, buf_size);

        int heading = 0;
        int exp = 10;
        int sp = 10;
        int current_load = 0;
        int max_load = 10;
        int unknown = 0x28;
        int paperdoll_under = 0x00;
        int paperdoll_rear = 0x00;
        int paperdoll_lear = 0x00;
        int paperdoll_neck = 0x00;
        int paperdoll_rfinger = 0x00;
        int paperdoll_lfinger = 0x00;
        int paperdoll_head = 0x00;
        int paperdoll_rhand = 0x00;
        int paperdoll_lhand = 0x00;
        int paperdoll_gloves = 0x00;
        int paperdoll_chest = 0x00;
        int paperdoll_legs = 0x00;
        int paperdoll_feet = 0x00;
        int paperdoll_back = 0x00;
        int paperdoll_lrhand = 0;
        int paperdoll_hair = 0x00;
        int p_atk = 20;
        int p_atk_speed = 20;
        int p_def = 20;
        int evasion_rate = 1;
        int accuracy = 10;
        int critical_hit = 1;
        int m_atk = 1;
        int m_atk_speed = 1;
        int m_def = 20;
        int pvp = 0;
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
        int access_level = 0;
        char title[] = { 'h', 0, 'i', 0, 0, 0 };
        int clan_id = 0;
        int clan_crest_id = 0;
        int ally_id = 0;
        int ally_crest_id = 0;
        int clan_leader = 0;
        char mount_type = 0;
        char private_store_type = 0;
        char dwarven_craft = 0;
        int pk_kills = 0;
        int pvp_kills = 10;
        short cubics = 0;
        char party_member = 0;
        int abnormal_effect = 0;
        char empty = 0;
        int clan_privileges = 0;
        int empty_int = 0;
        short recomendation_left = 0;
        short recomendation_have = 0;
        short inventory_limit = 10;
        int max_cp = 10;
        int cp = 10;
        char mounted = 0;
        int clan_crest_large_id = 0;
        char hero_symbol = 0;
        char hero = 0;
        int fish_x = 0;
        int fish_y = 0;
        int fish_z = 0;
        int name_color = 0xFFFFFF;

        struct Player* player = client_player(client);
        size_t player_name_len = strlen(player->character.name) + 1;

        l2_string name[28];
        int int_hp = (int) player->character.current_hp;
        int int_mp = (int) player->character.current_mp;
        int int_max_hp = (int) player->character.hp;
        int int_max_mp = (int) player->character.mp;

        memset(name, 0, 28);
        l2_string_from_char(name, player->character.name, player_name_len);

        byte_builder_append_int(buffer, &player->character.x);
        byte_builder_append_int(buffer, &player->character.y);
        byte_builder_append_int(buffer, &player->character.z);
        byte_builder_append_int(buffer, &heading);

        byte_builder_append_int(buffer, &player->character.id);

        byte_builder_append(
                buffer,
                name,
                l2_string_calculate_space_from_char(player_name_len)
        );

        byte_builder_append_int(buffer, &player->race_id);
        byte_builder_append_int(buffer, &player->character.sex);
        byte_builder_append_int(buffer, &player->class_id);
        byte_builder_append_int(buffer, &player->character.level);
        byte_builder_append_int(buffer, &exp);
        byte_builder_append_int(buffer, &player->character.str);
        byte_builder_append_int(buffer, &player->character.dex);
        byte_builder_append_int(buffer, &player->character.con);
        byte_builder_append_int(buffer, &player->character._int);
        byte_builder_append_int(buffer, &player->character.wit);
        byte_builder_append_int(buffer, &player->character.men);
        byte_builder_append_int(buffer, &int_max_hp);
        byte_builder_append_int(buffer, &int_hp);
        byte_builder_append_int(buffer, &int_max_mp);
        byte_builder_append_int(buffer, &int_mp);
        byte_builder_append_int(buffer, &sp);
        byte_builder_append_int(buffer, &current_load);
        byte_builder_append_int(buffer, &max_load);
        byte_builder_append_int(buffer, &unknown);

        byte_builder_append_int(buffer, &paperdoll_under);
        byte_builder_append_int(buffer, &paperdoll_rear);
        byte_builder_append_int(buffer, &paperdoll_lear);
        byte_builder_append_int(buffer, &paperdoll_neck);
        byte_builder_append_int(buffer, &paperdoll_rfinger);
        byte_builder_append_int(buffer, &paperdoll_lfinger);
        
        byte_builder_append_int(buffer, &paperdoll_head);

        byte_builder_append_int(buffer, &paperdoll_rhand);
        byte_builder_append_int(buffer, &paperdoll_lhand);
        byte_builder_append_int(buffer, &paperdoll_gloves);
        byte_builder_append_int(buffer, &paperdoll_chest);
        byte_builder_append_int(buffer, &paperdoll_legs);
        byte_builder_append_int(buffer, &paperdoll_feet);
        byte_builder_append_int(buffer, &paperdoll_back);
        byte_builder_append_int(buffer, &paperdoll_lrhand);
        byte_builder_append_int(buffer, &paperdoll_hair);

        byte_builder_append_int(buffer, &paperdoll_under);
        byte_builder_append_int(buffer, &paperdoll_rear);
        byte_builder_append_int(buffer, &paperdoll_lear);
        byte_builder_append_int(buffer, &paperdoll_neck);
        byte_builder_append_int(buffer, &paperdoll_rfinger);
        byte_builder_append_int(buffer, &paperdoll_lfinger);

        byte_builder_append_int(buffer, &paperdoll_head);
        byte_builder_append_int(buffer, &paperdoll_rhand);
        byte_builder_append_int(buffer, &paperdoll_lhand);
        byte_builder_append_int(buffer, &paperdoll_gloves);
        byte_builder_append_int(buffer, &paperdoll_chest);
        byte_builder_append_int(buffer, &paperdoll_legs);
        byte_builder_append_int(buffer, &paperdoll_feet);
        byte_builder_append_int(buffer, &paperdoll_back);
        byte_builder_append_int(buffer, &paperdoll_lrhand);
        byte_builder_append_int(buffer, &paperdoll_hair);

        byte_builder_append_int(buffer, &p_atk);
        byte_builder_append_int(buffer, &p_atk_speed);
        byte_builder_append_int(buffer, &p_def);
        byte_builder_append_int(buffer, &evasion_rate);
        byte_builder_append_int(buffer, &accuracy);
        byte_builder_append_int(buffer, &critical_hit);
        byte_builder_append_int(buffer, &m_atk);
        byte_builder_append_int(buffer, &m_atk_speed);
        byte_builder_append_int(buffer, &p_atk_speed);
        byte_builder_append_int(buffer, &m_def);

        byte_builder_append_int(buffer, &pvp);
        byte_builder_append_int(buffer, &karma);

        byte_builder_append_int(buffer, &run_speed);
        byte_builder_append_int(buffer, &walk_speed);
        byte_builder_append_int(buffer, &swim_run_speed);
        byte_builder_append_int(buffer, &swim_walk_speed);
        byte_builder_append_int(buffer, &fly_run_speed);
        byte_builder_append_int(buffer, &fly_walk_speed);
        byte_builder_append_int(buffer, &fly_run_speed);
        byte_builder_append_int(buffer, &fly_walk_speed);
        byte_builder_append_double(buffer, &movement_speed_multiplier);
        byte_builder_append_double(buffer, &atk_speed_multiplier);

        byte_builder_append_double(buffer, &collision_radius);
        byte_builder_append_double(buffer, &collision_height);

        byte_builder_append_int(buffer, &player->hair_style_id);
        byte_builder_append_int(buffer, &player->hair_color_id);
        byte_builder_append_int(buffer, &player->face);
        byte_builder_append_int(buffer, &access_level);

        byte_builder_append(buffer, title, sizeof(title));

        byte_builder_append_int(buffer, &clan_id);
        byte_builder_append_int(buffer, &clan_crest_id);
        byte_builder_append_int(buffer, &ally_id);
        byte_builder_append_int(buffer, &ally_crest_id);
        byte_builder_append_int(buffer, &clan_leader);
        byte_builder_append_char(buffer, &mount_type);
        byte_builder_append_char(buffer, &private_store_type);
        byte_builder_append_char(buffer, &dwarven_craft);
        byte_builder_append_int(buffer, &pk_kills);
        byte_builder_append_int(buffer, &pvp_kills);

        byte_builder_append_short(buffer, &cubics);

        byte_builder_append_char(buffer, &party_member);
        byte_builder_append_int(buffer, &abnormal_effect);
        byte_builder_append_char(buffer, &empty);
        byte_builder_append_int(buffer, &clan_privileges);

        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &empty_int);

        byte_builder_append_short(buffer, &recomendation_left);
        byte_builder_append_short(buffer, &recomendation_have);
        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_short(buffer, &inventory_limit);
        
        byte_builder_append_int(buffer, &player->class_id);

        byte_builder_append_int(buffer, &empty_int);
        byte_builder_append_int(buffer, &max_cp);
        byte_builder_append_int(buffer, &cp);

        byte_builder_append_char(buffer, &mounted);
        byte_builder_append_char(buffer, &empty);

        byte_builder_append_int(buffer, &clan_crest_large_id);
        byte_builder_append_char(buffer, &hero_symbol);
        byte_builder_append_char(buffer, &hero);

        byte_builder_append_char(buffer, &empty);
        byte_builder_append_int(buffer, &fish_x);
        byte_builder_append_int(buffer, &fish_y);
        byte_builder_append_int(buffer, &fish_z);
        byte_builder_append_int(buffer, &name_color);

        response = packet_builder_new(
                client,
                type,
                buffer,
                byte_builder_length(buffer)
        );

        client_free_mem(client, buf);
        client_free_mem(client, player);

        return response;
}
