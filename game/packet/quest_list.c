#include <assert.h>
#include <core/l2_packet.h>
#include <game/packet/quest_list.h>

l2_packet* game_packet_quest_list(struct L2Client* client)
{
        assert(client);

        l2_packet_type type = 0x80;
        unsigned char content[] = {
                0,
                0,
                0,
                0
        };

        return l2_client_create_packet(
                client,
                type,
                content,
                (unsigned short) sizeof(content)
        );
}
