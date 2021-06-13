#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_move.h"

void gs_packet_move(gs_packet_move_t *dest, gs_character_t *src)
{
        assert(dest);
        assert(src);

        dest->id     = src->id;
        dest->prev_x = src->x;
        dest->prev_y = src->y;
        dest->prev_z = src->z;
        dest->new_x  = src->target_x;
        dest->new_y  = src->target_y;
        dest->new_z  = src->target_z;
}

void gs_packet_move_pack(packet_t *dest, gs_packet_move_t *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);

        type = 0x01;

        packet_append_val(dest, type);
        packet_append_val(dest, src->id);
        packet_append_val(dest, src->new_x);
        packet_append_val(dest, src->new_y);
        packet_append_val(dest, src->new_z);
        packet_append_val(dest, src->prev_x);
        packet_append_val(dest, src->prev_y);
        packet_append_val(dest, src->prev_z);
}
