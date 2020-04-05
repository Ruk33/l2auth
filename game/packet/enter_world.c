#ifndef L2AUTH_LOGIN_GAME_PACKET_ENTER_WORLD
#define L2AUTH_LOGIN_GAME_PACKET_ENTER_WORLD

#include <core/l2_packet.c>
#include <core/byte_buffer.c>

l2_packet* game_packet_enter_world()
{
        l2_packet_type type = 0x04;
        struct ByteBuffer* buffer = byte_buffer_create();
        l2_packet *packet;

        int x = -71338;
        int y = 258271;
        int z = -3104;
        int heading = 0;
        int object_id = 0;
        char name[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };
        int race = 0;
        int sex = 0;
        int class = 0;
        int level = 1;
        int exp = 10;
        unsigned int str = 10;
        unsigned int dex = 10;
        unsigned int con = 10;
        unsigned int _int = 10;
        unsigned int wit = 10;
        unsigned int men = 10;
        unsigned int max_hp = 100;
        unsigned int hp = 100;
        unsigned int max_mp = 200;
        unsigned int mp = 100;
        unsigned int sp = 10;
        unsigned int current_load = 0;
        unsigned int max_load = 10;
        unsigned int unknown = 0x28;
        unsigned int paperdoll_under = 0;
        unsigned int paperdoll_rear = 0;
        unsigned int paperdoll_lear = 0;
        unsigned int paperdoll_neck = 0;
        unsigned int paperdoll_rfinger = 0;
        unsigned int paperdoll_lfinger = 0;
        unsigned int paperdoll_head = 0;
        unsigned int paperdoll_rhand = 0;
        unsigned int paperdoll_lhand = 0;
        unsigned int paperdoll_gloves = 0;
        unsigned int paperdoll_chest = 0;
        unsigned int paperdoll_legs = 0;
        unsigned int paperdoll_feet = 0;
        unsigned int paperdoll_back = 0;
        unsigned int paperdoll_lrhand = 0;
        unsigned int paperdoll_hair = 0;
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
        unsigned int run_speed = 10;
        unsigned int walk_speed = 10;
        unsigned int swim_run_speed = 10;
        unsigned int swim_walk_speed = 10;
        unsigned int fly_run_speed = 10;
        unsigned int fly_walk_speed = 10;
        double movement_speed_multiplier = 1;
        double atk_speed_multiplier = 1;
        double collision_radius = 9;
        double collision_height = 23;
        unsigned int hair_style = 0;
        unsigned int hair_color = 0;
        unsigned int face = 0;
        unsigned int access_level = 0;
        char title[] = { 'h', 0, 'i', 0, 0, 0 };
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
        unsigned int class_id = 0;
        unsigned int max_cp = 10;
        unsigned int cp = 10;
        unsigned char mounted[] = { 0 };
        unsigned int clan_crest_large_id = 0;
        unsigned char hero_symbol[] = { 0 };
        unsigned char hero[] = { 0 };
        unsigned int fish_x = 0;
        unsigned int fish_y = 0;
        unsigned int fish_z = 0;
        unsigned int name_color = 0;

        byte_buffer_append(buffer, &x, sizeof(x));
        byte_buffer_append(buffer, &y, sizeof(y));
        byte_buffer_append(buffer, &z, sizeof(z));
        byte_buffer_append(buffer, &heading, sizeof(heading));
        byte_buffer_append(buffer, &object_id, sizeof(object_id));
        byte_buffer_append(buffer, name, sizeof(name));
        byte_buffer_append(buffer, &race, sizeof(race));
        byte_buffer_append(buffer, &sex, sizeof(sex));
        byte_buffer_append(buffer, &class, sizeof(class));
        byte_buffer_append(buffer, &level, sizeof(level));
        byte_buffer_append(buffer, &exp, sizeof(exp));
        byte_buffer_append(buffer, &str, sizeof(str));
        byte_buffer_append(buffer, &dex, sizeof(dex));
        byte_buffer_append(buffer, &con, sizeof(con));
        byte_buffer_append(buffer, &_int, sizeof(_int));
        byte_buffer_append(buffer, &wit, sizeof(wit));
        byte_buffer_append(buffer, &men, sizeof(men));
        byte_buffer_append(buffer, &max_hp, sizeof(max_hp));
        byte_buffer_append(buffer, &hp, sizeof(hp));
        byte_buffer_append(buffer, &max_mp, sizeof(max_mp));
        byte_buffer_append(buffer, &mp, sizeof(mp));
        byte_buffer_append(buffer, &sp, sizeof(sp));
        byte_buffer_append(buffer, &current_load, sizeof(current_load));
        byte_buffer_append(buffer, &max_load, sizeof(max_load));
        byte_buffer_append(buffer, &unknown, sizeof(unknown));

        byte_buffer_append(buffer, &paperdoll_under, sizeof(paperdoll_under));
        byte_buffer_append(buffer, &paperdoll_rear, sizeof(paperdoll_rear));
        byte_buffer_append(buffer, &paperdoll_lear, sizeof(paperdoll_lear));
        byte_buffer_append(buffer, &paperdoll_neck, sizeof(paperdoll_neck));
        byte_buffer_append(buffer, &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_buffer_append(buffer, &paperdoll_lfinger, sizeof(paperdoll_lfinger));
        
        byte_buffer_append(buffer, &paperdoll_head, sizeof(paperdoll_head));

        byte_buffer_append(buffer, &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_buffer_append(buffer, &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_buffer_append(buffer, &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_buffer_append(buffer, &paperdoll_chest, sizeof(paperdoll_chest));
        byte_buffer_append(buffer, &paperdoll_legs, sizeof(paperdoll_legs));
        byte_buffer_append(buffer, &paperdoll_feet, sizeof(paperdoll_feet));
        byte_buffer_append(buffer, &paperdoll_back, sizeof(paperdoll_back));
        byte_buffer_append(buffer, &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_buffer_append(buffer, &paperdoll_hair, sizeof(paperdoll_hair));

        byte_buffer_append(buffer, &paperdoll_under, sizeof(paperdoll_under));
        byte_buffer_append(buffer, &paperdoll_rear, sizeof(paperdoll_rear));
        byte_buffer_append(buffer, &paperdoll_lear, sizeof(paperdoll_lear));
        byte_buffer_append(buffer, &paperdoll_neck, sizeof(paperdoll_neck));
        byte_buffer_append(buffer, &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_buffer_append(buffer, &paperdoll_lfinger, sizeof(paperdoll_lfinger));

        byte_buffer_append(buffer, &paperdoll_head, sizeof(paperdoll_head));
        byte_buffer_append(buffer, &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_buffer_append(buffer, &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_buffer_append(buffer, &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_buffer_append(buffer, &paperdoll_chest, sizeof(paperdoll_chest));
        byte_buffer_append(buffer, &paperdoll_legs, sizeof(paperdoll_legs));
        byte_buffer_append(buffer, &paperdoll_feet, sizeof(paperdoll_feet));
        byte_buffer_append(buffer, &paperdoll_back, sizeof(paperdoll_back));
        byte_buffer_append(buffer, &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_buffer_append(buffer, &paperdoll_hair, sizeof(paperdoll_hair));

        byte_buffer_append(buffer, &p_atk, sizeof(p_atk));
        byte_buffer_append(buffer, &p_atk_speed, sizeof(p_atk_speed));
        byte_buffer_append(buffer, &p_def, sizeof(p_def));
        byte_buffer_append(buffer, &evasion_rate, sizeof(evasion_rate));
        byte_buffer_append(buffer, &accuracy, sizeof(accuracy));
        byte_buffer_append(buffer, &critical_hit, sizeof(critical_hit));
        byte_buffer_append(buffer, &m_atk, sizeof(m_atk));
        byte_buffer_append(buffer, &m_atk_speed, sizeof(m_atk_speed));
        byte_buffer_append(buffer, &p_atk_speed, sizeof(p_atk_speed));
        byte_buffer_append(buffer, &m_def, sizeof(m_def));

        byte_buffer_append(buffer, &pvp, sizeof(pvp));
        byte_buffer_append(buffer, &karma, sizeof(karma));

        byte_buffer_append(buffer, &run_speed, sizeof(run_speed));
        byte_buffer_append(buffer, &walk_speed, sizeof(walk_speed));
        byte_buffer_append(buffer, &swim_run_speed, sizeof(swim_run_speed));
        byte_buffer_append(buffer, &swim_walk_speed, sizeof(swim_walk_speed));
        byte_buffer_append(buffer, &fly_run_speed, sizeof(fly_run_speed));
        byte_buffer_append(buffer, &fly_walk_speed, sizeof(fly_walk_speed));
        byte_buffer_append(buffer, &fly_run_speed, sizeof(fly_run_speed));
        byte_buffer_append(buffer, &fly_walk_speed, sizeof(fly_walk_speed));
        byte_buffer_append(buffer, &movement_speed_multiplier, sizeof(movement_speed_multiplier));
        byte_buffer_append(buffer, &atk_speed_multiplier, sizeof(atk_speed_multiplier));

        byte_buffer_append(buffer, &collision_radius, sizeof(collision_radius));
        byte_buffer_append(buffer, &collision_height, sizeof(collision_height));

        byte_buffer_append(buffer, &hair_style, sizeof(hair_style));
        byte_buffer_append(buffer, &hair_color, sizeof(hair_color));
        byte_buffer_append(buffer, &face, sizeof(face));
        byte_buffer_append(buffer, &access_level, sizeof(access_level));

        byte_buffer_append(buffer, title, sizeof(title));

        byte_buffer_append(buffer, &clan_id, sizeof(clan_id));
        byte_buffer_append(buffer, &clan_crest_id, sizeof(clan_crest_id));
        byte_buffer_append(buffer, &ally_id, sizeof(ally_id));
        byte_buffer_append(buffer, &ally_crest_id, sizeof(ally_crest_id));
        byte_buffer_append(buffer, &clan_leader, sizeof(clan_leader));
        byte_buffer_append(buffer, &mount_type, sizeof(mount_type));
        byte_buffer_append(buffer, &private_store_type, sizeof(private_store_type));
        byte_buffer_append(buffer, &dwarven_craft, sizeof(dwarven_craft));
        byte_buffer_append(buffer, &pk_kills, sizeof(pk_kills));
        byte_buffer_append(buffer, &pvp_kills, sizeof(pvp_kills));

        byte_buffer_append(buffer, &cubics, sizeof(cubics));

        byte_buffer_append(buffer, &party_member, sizeof(party_member));
        byte_buffer_append(buffer, &abnormal_effect, sizeof(abnormal_effect));
        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &clan_privileges, sizeof(clan_privileges));

        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));

        byte_buffer_append(buffer, &recomendation_left, sizeof(recomendation_left));
        byte_buffer_append(buffer, &recomendation_have, sizeof(recomendation_have));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &inventory_limit, sizeof(inventory_limit));
        
        byte_buffer_append(buffer, &class_id, sizeof(class_id));
        byte_buffer_append(buffer, &empty_int, sizeof(empty_int));
        byte_buffer_append(buffer, &max_cp, sizeof(max_cp));
        byte_buffer_append(buffer, &cp, sizeof(cp));

        byte_buffer_append(buffer, &mounted, sizeof(mounted));

        byte_buffer_append(buffer, &clan_crest_large_id, sizeof(clan_crest_large_id));
        byte_buffer_append(buffer, &hero_symbol, sizeof(hero_symbol));
        byte_buffer_append(buffer, &hero, sizeof(hero));

        byte_buffer_append(buffer, &empty, sizeof(empty));
        byte_buffer_append(buffer, &fish_x, sizeof(fish_x));
        byte_buffer_append(buffer, &fish_y, sizeof(fish_y));
        byte_buffer_append(buffer, &fish_z, sizeof(fish_z));
        byte_buffer_append(buffer, &name_color, sizeof(name_color));

        packet = l2_packet_new(
                type,
                byte_buffer_content(buffer),
                (unsigned short) byte_buffer_size(buffer)
        );

        byte_buffer_free(buffer);

        return packet;
}

#endif
