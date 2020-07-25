#include <string.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <core/l2_string.h>
#include "../../client.h"
#include "../../entity/pc.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *enter_world_response(struct Client *client)
{
        l2_packet_type type = 0x04;
        l2_packet *response;

        size_t buf_size = byte_builder_calculate_size(1024);
        byte_builder *buf = client_alloc_mem(client, buf_size);
        byte_builder* buffer = byte_builder_init(buf, buf_size);

        int heading = 0;
        int exp = 10;
        unsigned int sp = 10;
        unsigned int current_load = 0;
        unsigned int max_load = 10;
        unsigned int unknown = 0x28;
        unsigned int paperdoll_under = 0x00;
        unsigned int paperdoll_rear = 0x00;
        unsigned int paperdoll_lear = 0x00;
        unsigned int paperdoll_neck = 0x00;
        unsigned int paperdoll_rfinger = 0x00;
        unsigned int paperdoll_lfinger = 0x00;
        unsigned int paperdoll_head = 0x00;
        unsigned int paperdoll_rhand = 0x00;
        unsigned int paperdoll_lhand = 0x00;
        unsigned int paperdoll_gloves = 0x00;
        unsigned int paperdoll_chest = 0x00;
        unsigned int paperdoll_legs = 0x00;
        unsigned int paperdoll_feet = 0x00;
        unsigned int paperdoll_back = 0x00;
        unsigned int paperdoll_lrhand = 0;
        unsigned int paperdoll_hair = 0x00;
        unsigned int p_atk = 20;
        unsigned int p_atk_speed = 20;
        unsigned int p_def = 20;
        unsigned int evasion_rate = 1;
        unsigned int accuracy = 10;
        unsigned int critical_hit = 1;
        unsigned int m_atk = 1;
        unsigned int m_atk_speed = 1;
        unsigned int m_def = 20;
        unsigned int pvp = 0;
        unsigned int karma = 0;
        unsigned int run_speed = 120;
        unsigned int walk_speed = 100;
        unsigned int swim_run_speed = 10;
        unsigned int swim_walk_speed = 10;
        unsigned int fly_run_speed = 10;
        unsigned int fly_walk_speed = 10;
        double movement_speed_multiplier = 1;
        double atk_speed_multiplier = 1;
        double collision_radius = 9;
        double collision_height = 23;
        unsigned int access_level = 0;
        unsigned char title[] = { 'h', 0, 'i', 0, 0, 0 };
        unsigned int clan_id = 0;
        unsigned int clan_crest_id = 0;
        unsigned int ally_id = 0;
        unsigned int ally_crest_id = 0;
        unsigned int clan_leader = 0;
        unsigned char mount_type[] = { 0 };
        unsigned char private_store_type[] = { 0 };
        unsigned char dwarven_craft[] = { 0 };
        unsigned int pk_kills = 0;
        unsigned int pvp_kills = 10;
        unsigned short cubics = 0;
        unsigned char party_member[] = { 0 };
        unsigned int abnormal_effect = 0;
        unsigned char empty[] = { 0 };
        unsigned int clan_privileges = 0;
        unsigned int empty_int = 0;
        unsigned short recomendation_left = 0;
        unsigned short recomendation_have = 0;
        unsigned short inventory_limit = 10;
        unsigned int max_cp = 10;
        unsigned int cp = 10;
        unsigned char mounted[] = { 0 };
        unsigned int clan_crest_large_id = 0;
        unsigned char hero_symbol[] = { 0 };
        unsigned char hero[] = { 0 };
        unsigned int fish_x = 0;
        unsigned int fish_y = 0;
        unsigned int fish_z = 0;
        unsigned int name_color = 4294967295;

        struct Pc* player = client_character(client);

        l2_string name[28];
        int int_hp = (int) player->character.current_hp;
        int int_mp = (int) player->character.current_mp;
        int int_max_hp = (int) player->character.hp;
        int int_max_mp = (int) player->character.mp;

        memset(name, 0, 28);
        l2_string_from_char(name, player->character.name, strlen(player->character.name));

        byte_builder_append(
                buffer,
                &player->character.x,
                sizeof(player->character.x)
        );
        byte_builder_append(
                buffer,
                &player->character.y,
                sizeof(player->character.y)
        );
        byte_builder_append(
                buffer,
                &player->character.z,
                sizeof(player->character.z)
        );
        byte_builder_append(
                buffer,
                &heading,
                sizeof(heading)
        );

        byte_builder_append(
                buffer,
                &player->character.id,
                sizeof(player->character.id)
        );

        byte_builder_append(
                buffer,
                name,
                l2_string_calculate_space_from_char(strlen(player->character.name) + 1)
        );

        byte_builder_append(
                buffer,
                &player->race_id,
                sizeof(player->race_id)
        );
        byte_builder_append(
                buffer,
                &player->character.sex,
                sizeof(player->character.sex)
        );
        byte_builder_append(
                buffer,
                &player->class_id,
                sizeof(player->class_id)
        );
        byte_builder_append(
                buffer,
                &player->character.level,
                sizeof(player->character.level)
        );
        byte_builder_append(buffer, &exp, sizeof(exp));
        byte_builder_append(
                buffer,
                &player->character.str,
                sizeof(player->character.str)
        );
        byte_builder_append(
                buffer,
                &player->character.dex,
                sizeof(player->character.dex)
        );
        byte_builder_append(
                buffer,
                &player->character.con,
                sizeof(player->character.con)
        );
        byte_builder_append(
                buffer,
                &player->character._int,
                sizeof(player->character._int)
        );
        byte_builder_append(
                buffer,
                &player->character.wit,
                sizeof(player->character.wit)
        );
        byte_builder_append(
                buffer,
                &player->character.men,
                sizeof(player->character.men)
        );
        byte_builder_append(
                buffer,
                &int_max_hp,
                sizeof(int_max_hp)
        );
        byte_builder_append(
                buffer,
                &int_hp,
                sizeof(int_hp)
        );
        byte_builder_append(
                buffer,
                &int_max_mp,
                sizeof(int_max_mp)
        );
        byte_builder_append(
                buffer,
                &int_mp,
                sizeof(int_mp)
        );
        byte_builder_append(buffer, &sp, sizeof(sp));
        byte_builder_append(buffer, &current_load, sizeof(current_load));
        byte_builder_append(buffer, &max_load, sizeof(max_load));
        byte_builder_append(buffer, &unknown, sizeof(unknown));

        byte_builder_append(buffer, &paperdoll_under, sizeof(paperdoll_under));
        byte_builder_append(buffer, &paperdoll_rear, sizeof(paperdoll_rear));
        byte_builder_append(buffer, &paperdoll_lear, sizeof(paperdoll_lear));
        byte_builder_append(buffer, &paperdoll_neck, sizeof(paperdoll_neck));
        byte_builder_append(buffer, &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_builder_append(buffer, &paperdoll_lfinger, sizeof(paperdoll_lfinger));
        
        byte_builder_append(buffer, &paperdoll_head, sizeof(paperdoll_head));

        byte_builder_append(buffer, &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_builder_append(buffer, &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_builder_append(buffer, &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_builder_append(buffer, &paperdoll_chest, sizeof(paperdoll_chest));
        byte_builder_append(buffer, &paperdoll_legs, sizeof(paperdoll_legs));
        byte_builder_append(buffer, &paperdoll_feet, sizeof(paperdoll_feet));
        byte_builder_append(buffer, &paperdoll_back, sizeof(paperdoll_back));
        byte_builder_append(buffer, &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_builder_append(buffer, &paperdoll_hair, sizeof(paperdoll_hair));

        byte_builder_append(buffer, &paperdoll_under, sizeof(paperdoll_under));
        byte_builder_append(buffer, &paperdoll_rear, sizeof(paperdoll_rear));
        byte_builder_append(buffer, &paperdoll_lear, sizeof(paperdoll_lear));
        byte_builder_append(buffer, &paperdoll_neck, sizeof(paperdoll_neck));
        byte_builder_append(buffer, &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_builder_append(buffer, &paperdoll_lfinger, sizeof(paperdoll_lfinger));

        byte_builder_append(buffer, &paperdoll_head, sizeof(paperdoll_head));
        byte_builder_append(buffer, &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_builder_append(buffer, &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_builder_append(buffer, &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_builder_append(buffer, &paperdoll_chest, sizeof(paperdoll_chest));
        byte_builder_append(buffer, &paperdoll_legs, sizeof(paperdoll_legs));
        byte_builder_append(buffer, &paperdoll_feet, sizeof(paperdoll_feet));
        byte_builder_append(buffer, &paperdoll_back, sizeof(paperdoll_back));
        byte_builder_append(buffer, &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_builder_append(buffer, &paperdoll_hair, sizeof(paperdoll_hair));

        byte_builder_append(buffer, &p_atk, sizeof(p_atk));
        byte_builder_append(buffer, &p_atk_speed, sizeof(p_atk_speed));
        byte_builder_append(buffer, &p_def, sizeof(p_def));
        byte_builder_append(buffer, &evasion_rate, sizeof(evasion_rate));
        byte_builder_append(buffer, &accuracy, sizeof(accuracy));
        byte_builder_append(buffer, &critical_hit, sizeof(critical_hit));
        byte_builder_append(buffer, &m_atk, sizeof(m_atk));
        byte_builder_append(buffer, &m_atk_speed, sizeof(m_atk_speed));
        byte_builder_append(buffer, &p_atk_speed, sizeof(p_atk_speed));
        byte_builder_append(buffer, &m_def, sizeof(m_def));

        byte_builder_append(buffer, &pvp, sizeof(pvp));
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

        byte_builder_append(
                buffer,
                &player->hair_style_id,
                sizeof(player->hair_style_id)
        );
        byte_builder_append(
                buffer,
                &player->hair_color_id,
                sizeof(player->hair_color_id)
        );
        byte_builder_append(
                buffer,
                &player->face,
                sizeof(player->face)
        );
        byte_builder_append(buffer, &access_level, sizeof(access_level));

        byte_builder_append(buffer, title, sizeof(title));

        byte_builder_append(buffer, &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, &clan_crest_id, sizeof(clan_crest_id));
        byte_builder_append(buffer, &ally_id, sizeof(ally_id));
        byte_builder_append(buffer, &ally_crest_id, sizeof(ally_crest_id));
        byte_builder_append(buffer, &clan_leader, sizeof(clan_leader));
        byte_builder_append(buffer, &mount_type, sizeof(mount_type));
        byte_builder_append(buffer, &private_store_type, sizeof(private_store_type));
        byte_builder_append(buffer, &dwarven_craft, sizeof(dwarven_craft));
        byte_builder_append(buffer, &pk_kills, sizeof(pk_kills));
        byte_builder_append(buffer, &pvp_kills, sizeof(pvp_kills));

        byte_builder_append(buffer, &cubics, sizeof(cubics));

        byte_builder_append(buffer, &party_member, sizeof(party_member));
        byte_builder_append(buffer, &abnormal_effect, sizeof(abnormal_effect));
        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &clan_privileges, sizeof(clan_privileges));

        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));

        byte_builder_append(buffer, &recomendation_left, sizeof(recomendation_left));
        byte_builder_append(buffer, &recomendation_have, sizeof(recomendation_have));
        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &inventory_limit, sizeof(inventory_limit));
        
        byte_builder_append(
                buffer,
                &player->class_id,
                sizeof(player->class_id)
        );

        byte_builder_append(buffer, &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, &max_cp, sizeof(max_cp));
        byte_builder_append(buffer, &cp, sizeof(cp));

        byte_builder_append(buffer, &mounted, sizeof(mounted));
        byte_builder_append(buffer, &empty, sizeof(empty));

        byte_builder_append(buffer, &clan_crest_large_id, sizeof(clan_crest_large_id));
        byte_builder_append(buffer, &hero_symbol, sizeof(hero_symbol));
        byte_builder_append(buffer, &hero, sizeof(hero));

        byte_builder_append(buffer, &empty, sizeof(empty));
        byte_builder_append(buffer, &fish_x, sizeof(fish_x));
        byte_builder_append(buffer, &fish_y, sizeof(fish_y));
        byte_builder_append(buffer, &fish_z, sizeof(fish_z));
        byte_builder_append(buffer, &name_color, sizeof(name_color));

        response = packet_builder_new(client, type, buffer, byte_builder_length(buffer));

        client_free_mem(client, buf);
        client_free_mem(client, player);

        return response;
}
