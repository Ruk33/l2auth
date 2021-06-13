#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/server.h"
#include "include/ls_packet_server_list.h"

void ls_packet_server_list_add(ls_packet_server_list_t *dest, server_t *src)
{
        assert(dest);
        assert(src);
        assert(dest->count < arr_size(dest->servers));

        dest->servers[dest->count] = *src;
        dest->count += 1;
}

void ls_packet_server_list_pack(packet_t *dest, ls_packet_server_list_t *src)
{
        byte_t type = 0;

        type = 0x04;

        assert(dest);
        assert(src);

        packet_append_val(dest, type);
        packet_append_val(dest, src->count);
        packet_append_val(dest, src->reserved);

        for (u8_t i = 0; i < src->count; i += 1) {
                packet_append_val(dest, src->servers[i].id);
                packet_append(dest, src->servers[i].ip);
                packet_append_val(dest, src->servers[i].port);
                packet_append_val(dest, src->servers[i].age_limit);
                packet_append_val(dest, src->servers[i].pvp);
                packet_append_val(dest, src->servers[i].players);
                packet_append_val(dest, src->servers[i].max_players);
                packet_append_val(dest, src->servers[i].status);
                packet_append_val(dest, src->servers[i].extra);
                packet_append_val(dest, src->servers[i].brackets);
        }
}
