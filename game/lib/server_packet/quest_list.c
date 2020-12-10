#include "../headers.h"
#include "quest_list.h"

void server_packet_quest_list(packet *dest)
{
        assert(dest);

        packet_type type = 0x80;

        byte buf[SERVER_PACKET_QUEST_LIST_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketQuestList quest_list_packet = {0};

        BYTE_WRITE(p, quest_list_packet.empty, sizeof(quest_list_packet.empty));

        packet_build(dest, type, buf, (size_t) (p - buf));
}
