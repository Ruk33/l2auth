#include <assert.h>
#include <stdlib.h>
#include <log/log.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/byte_builder.h>
#include <login/dto/session_key.h>
#include <game/packet/char_selected.h>

l2_packet* game_packet_char_selected(struct L2Client* client)
{
        assert(client);

        l2_packet_type type = 0x15;
        byte_builder* buffer = l2_client_byte_builder(client, 1000);

        struct LoginDtoSessionKey* session = l2_client_session(client);
        int playOK1 = session->playOK1;
        unsigned char name[] = { 'r', 0, 'u', 0, 'k', 0, 'e', 0, 0, 0 };
        unsigned int char_id = 0x00;
        unsigned char title[] = { 0, 0 };
        unsigned int clan_id = 0;
        unsigned int empty = 0;
        unsigned int sex = 0x00;
        unsigned int race = 0x00;
        unsigned int class = 0x00;
        unsigned int active = 0x01;
        int x = -71338;
        int y = 258271;
        int z = -3104;
        double current_hp = 200;
        double current_mp = 100;
        unsigned int sp = 20;
        unsigned int exp = 10;
        unsigned int level = 1;
        unsigned int _int = 10;
        unsigned int str = 10;
        unsigned int con = 10;
        unsigned int men = 10;
        unsigned int dex = 10;
        unsigned int wit = 10;
        unsigned char* thirty_empty = l2_client_alloc_temp_mem(client, 30 * sizeof(unsigned char));
        unsigned int game_time = 10;

        byte_builder_append(buffer, name, sizeof(name));
        byte_builder_append(buffer, (unsigned char *) &char_id, sizeof(char_id));
        byte_builder_append(buffer, title, sizeof(title));
        byte_builder_append(buffer, (unsigned char *) &playOK1, sizeof(playOK1));
        byte_builder_append(buffer, (unsigned char *) &clan_id, sizeof(clan_id));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &sex, sizeof(sex));
        byte_builder_append(buffer, (unsigned char *) &race, sizeof(race));
        byte_builder_append(buffer, (unsigned char *) &class, sizeof(class));
        byte_builder_append(buffer, (unsigned char *) &active, sizeof(active));
        byte_builder_append(buffer, (unsigned char *) &x, sizeof(x));
        byte_builder_append(buffer, (unsigned char *) &y, sizeof(y));
        byte_builder_append(buffer, (unsigned char *) &z, sizeof(z));
        byte_builder_append(buffer, (unsigned char *) &current_hp, sizeof(current_hp));
        byte_builder_append(buffer, (unsigned char *) &current_mp, sizeof(current_mp));
        byte_builder_append(buffer, (unsigned char *) &sp, sizeof(sp));
        byte_builder_append(buffer, (unsigned char *) &exp, sizeof(exp));
        byte_builder_append(buffer, (unsigned char *) &level, sizeof(level));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &_int, sizeof(_int));
        byte_builder_append(buffer, (unsigned char *) &str, sizeof(str));
        byte_builder_append(buffer, (unsigned char *) &con, sizeof(con));
        byte_builder_append(buffer, (unsigned char *) &men, sizeof(men));
        byte_builder_append(buffer, (unsigned char *) &dex, sizeof(dex));
        byte_builder_append(buffer, (unsigned char *) &wit, sizeof(wit));
        byte_builder_append(buffer, thirty_empty, 30);
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &game_time, sizeof(game_time));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));
        byte_builder_append(buffer, (unsigned char *) &empty, sizeof(empty));

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
