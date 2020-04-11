#include <assert.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/byte_builder.h>
#include <game/packet/enter_world.h>

l2_packet* game_packet_enter_world(struct L2Client* client)
{
        assert(client);
        l2_packet_type type = 0x04;
        byte_builder* buffer = l2_client_byte_builder(client, 1000);

        int x = -71338;
        int y = 258271;
        int z = -3104;
        int heading = 0;
        int object_id = 0;
        unsigned char name[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };
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

        byte_builder_append(buffer, (unsigned char *) &x, sizeof(x));
        byte_builder_append(buffer, (unsigned char *) &y, sizeof(y));
        byte_builder_append(buffer, (unsigned char *) &z, sizeof(z));
        byte_builder_append(buffer, (unsigned char *) &heading, sizeof(heading));
        byte_builder_append(buffer, (unsigned char *) &object_id, sizeof(object_id));
        byte_builder_append(buffer, name, sizeof(name));
        byte_builder_append(buffer, (unsigned char *) &race, sizeof(race));
        byte_builder_append(buffer, (unsigned char *) &sex, sizeof(sex));
        byte_builder_append(buffer, (unsigned char *) &class, sizeof(class));
        byte_builder_append(buffer, (unsigned char *) &level, sizeof(level));
        byte_builder_append(buffer, (unsigned char *) &exp, sizeof(exp));
        byte_builder_append(buffer, (unsigned char *) &str, sizeof(str));
        byte_builder_append(buffer, (unsigned char *) &dex, sizeof(dex));
        byte_builder_append(buffer, (unsigned char *) &con, sizeof(con));
        byte_builder_append(buffer, (unsigned char *) &_int, sizeof(_int));
        byte_builder_append(buffer, (unsigned char *) &wit, sizeof(wit));
        byte_builder_append(buffer, (unsigned char *) &men, sizeof(men));
        byte_builder_append(buffer, (unsigned char *) &max_hp, sizeof(max_hp));
        byte_builder_append(buffer, (unsigned char *) &hp, sizeof(hp));
        byte_builder_append(buffer, (unsigned char *) &max_mp, sizeof(max_mp));
        byte_builder_append(buffer, (unsigned char *) &mp, sizeof(mp));
        byte_builder_append(buffer, (unsigned char *) &sp, sizeof(sp));
        byte_builder_append(buffer, (unsigned char *) &current_load, sizeof(current_load));
        byte_builder_append(buffer, (unsigned char *) &max_load, sizeof(max_load));
        byte_builder_append(buffer, (unsigned char *) &unknown, sizeof(unknown));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_under, sizeof(paperdoll_under));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rear, sizeof(paperdoll_rear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lear, sizeof(paperdoll_lear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_neck, sizeof(paperdoll_neck));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lfinger, sizeof(paperdoll_lfinger));
        
        byte_builder_append(buffer, (unsigned char *) &paperdoll_head, sizeof(paperdoll_head));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_chest, sizeof(paperdoll_chest));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_legs, sizeof(paperdoll_legs));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_feet, sizeof(paperdoll_feet));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_back, sizeof(paperdoll_back));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_hair, sizeof(paperdoll_hair));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_under, sizeof(paperdoll_under));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rear, sizeof(paperdoll_rear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lear, sizeof(paperdoll_lear));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_neck, sizeof(paperdoll_neck));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rfinger, sizeof(paperdoll_rfinger));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lfinger, sizeof(paperdoll_lfinger));

        byte_builder_append(buffer, (unsigned char *) &paperdoll_head, sizeof(paperdoll_head));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_rhand, sizeof(paperdoll_rhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lhand, sizeof(paperdoll_lhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_gloves, sizeof(paperdoll_gloves));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_chest, sizeof(paperdoll_chest));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_legs, sizeof(paperdoll_legs));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_feet, sizeof(paperdoll_feet));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_back, sizeof(paperdoll_back));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_lrhand, sizeof(paperdoll_lrhand));
        byte_builder_append(buffer, (unsigned char *) &paperdoll_hair, sizeof(paperdoll_hair));

        byte_builder_append(buffer, (unsigned char *) &p_atk, sizeof(p_atk));
        byte_builder_append(buffer, (unsigned char *) &p_atk_speed, sizeof(p_atk_speed));
        byte_builder_append(buffer, (unsigned char *) &p_def, sizeof(p_def));
        byte_builder_append(buffer, (unsigned char *) &evasion_rate, sizeof(evasion_rate));
        byte_builder_append(buffer, (unsigned char *) &accuracy, sizeof(accuracy));
        byte_builder_append(buffer, (unsigned char *) &critical_hit, sizeof(critical_hit));
        byte_builder_append(buffer, (unsigned char *) &m_atk, sizeof(m_atk));
        byte_builder_append(buffer, (unsigned char *) &m_atk_speed, sizeof(m_atk_speed));
        byte_builder_append(buffer, (unsigned char *) &p_atk_speed, sizeof(p_atk_speed));
        byte_builder_append(buffer, (unsigned char *) &m_def, sizeof(m_def));

        byte_builder_append(buffer, (unsigned char *) &pvp, sizeof(pvp));
        byte_builder_append(buffer, (unsigned char *) &karma, sizeof(karma));

        byte_builder_append(buffer, (unsigned char *) &run_speed, sizeof(run_speed));
        byte_builder_append(buffer, (unsigned char *) &walk_speed, sizeof(walk_speed));
        byte_builder_append(buffer, (unsigned char *) &swim_run_speed, sizeof(swim_run_speed));
        byte_builder_append(buffer, (unsigned char *) &swim_walk_speed, sizeof(swim_walk_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_run_speed, sizeof(fly_run_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_walk_speed, sizeof(fly_walk_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_run_speed, sizeof(fly_run_speed));
        byte_builder_append(buffer, (unsigned char *) &fly_walk_speed, sizeof(fly_walk_speed));
        byte_builder_append(buffer, (unsigned char *) &movement_speed_multiplier, sizeof(movement_speed_multiplier));
        byte_builder_append(buffer, (unsigned char *) &atk_speed_multiplier, sizeof(atk_speed_multiplier));

        byte_builder_append(buffer, (unsigned char *) &collision_radius, sizeof(collision_radius));
        byte_builder_append(buffer, (unsigned char *) &collision_height, sizeof(collision_height));

        byte_builder_append(buffer, (unsigned char *) &hair_style, sizeof(hair_style));
        byte_builder_append(buffer, (unsigned char *) &hair_color, sizeof(hair_color));
        byte_builder_append(buffer, (unsigned char *) &face, sizeof(face));
        byte_builder_append(buffer, (unsigned char *) &access_level, sizeof(access_level));

        byte_builder_append(buffer, title, sizeof(title));

        byte_builder_append(buffer, (unsigned char *) &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, (unsigned char *) &clan_crest_id, sizeof(clan_crest_id));
        byte_builder_append(buffer, (unsigned char *) &ally_id, sizeof(ally_id));
        byte_builder_append(buffer, (unsigned char *) &ally_crest_id, sizeof(ally_crest_id));
        byte_builder_append(buffer, (unsigned char *) &clan_leader, sizeof(clan_leader));
        byte_builder_append(buffer, (unsigned char *) &mount_type, sizeof(mount_type));
        byte_builder_append(buffer, (unsigned char *) &private_store_type, sizeof(private_store_type));
        byte_builder_append(buffer, (unsigned char *) &dwarven_craft, sizeof(dwarven_craft));
        byte_builder_append(buffer, (unsigned char *) &pk_kills, sizeof(pk_kills));
        byte_builder_append(buffer, (unsigned char *) &pvp_kills, sizeof(pvp_kills));

        byte_builder_append(buffer, (unsigned char *) &cubics, sizeof(cubics));

        byte_builder_append(buffer, (unsigned char *) &party_member, sizeof(party_member));
        byte_builder_append(buffer, (unsigned char *) &abnormal_effect, sizeof(abnormal_effect));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &clan_privileges, sizeof(clan_privileges));

        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));

        byte_builder_append(buffer, (unsigned char *) &recomendation_left, sizeof(recomendation_left));
        byte_builder_append(buffer, (unsigned char *) &recomendation_have, sizeof(recomendation_have));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &inventory_limit, sizeof(inventory_limit));
        
        byte_builder_append(buffer, (unsigned char *) &class_id, sizeof(class_id));
        byte_builder_append(buffer, (unsigned char *) &empty_int, sizeof(empty_int));
        byte_builder_append(buffer, (unsigned char *) &max_cp, sizeof(max_cp));
        byte_builder_append(buffer, (unsigned char *) &cp, sizeof(cp));

        byte_builder_append(buffer, (unsigned char *) &mounted, sizeof(mounted));

        byte_builder_append(buffer, (unsigned char *) &clan_crest_large_id, sizeof(clan_crest_large_id));
        byte_builder_append(buffer, (unsigned char *) &hero_symbol, sizeof(hero_symbol));
        byte_builder_append(buffer, (unsigned char *) &hero, sizeof(hero));

        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &fish_x, sizeof(fish_x));
        byte_builder_append(buffer, (unsigned char *) &fish_y, sizeof(fish_y));
        byte_builder_append(buffer, (unsigned char *) &fish_z, sizeof(fish_z));
        byte_builder_append(buffer, (unsigned char *) &name_color, sizeof(name_color));

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
