#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <data_structures/list.h>
#include "../byte_write.h"
#include "../packet.h"
#include "../storage/game_server.h"
#include "server_list.h"

void server_packet_server_list(unsigned char *dest, struct List *servers)
{
        assert(dest);
        assert(servers);

        unsigned char type = 0x04;
        struct ServerPacketServerList packet = {0};

        struct ListEntry *iterator = list_get_iterator(servers);
        struct StorageGameServer *server = NULL;

        unsigned char buf[SERVER_PACKET_SERVER_LIST_SIZE] = {0};
        unsigned char *p = buf;

        // We may want to consider adding a size
        // to the list api...
        while (iterator)
        {
                packet.count += 1;
                iterator = list_get_next(iterator);
        }

        BYTE_WRITE(p, &packet.count, sizeof(packet.count));
        BYTE_WRITE(p, &packet.reserved_space, sizeof(packet.reserved_space));

        iterator = list_get_iterator(servers);

        while (iterator)
        {
                server = list_get_value(iterator);

                BYTE_WRITE_VAL(p, server->id);
                BYTE_WRITE(p, server->ip, sizeof(server->ip));
                BYTE_WRITE_VAL(p, server->port);
                BYTE_WRITE_VAL(p, server->age_limit);
                BYTE_WRITE_VAL(p, server->pvp);
                BYTE_WRITE_VAL(p, server->players);
                BYTE_WRITE_VAL(p, server->max_players);
                BYTE_WRITE_VAL(p, server->status);
                BYTE_WRITE_VAL(p, server->extra);
                BYTE_WRITE_VAL(p, server->brackets);

                iterator = list_get_next(iterator);
        }

        packet_build(dest, type, buf, p - buf);
}
