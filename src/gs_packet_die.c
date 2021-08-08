#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_packet_die.h"

void gs_packet_die_add_options(
        struct gs_packet_die *dest,
        struct gs_character *src)
{
        assert(dest);
        assert(src);

        // Todo: add other options when clans are implemented

        dest->obj_id     = src->id;
        dest->to_village = 0x01;
        dest->to_fixed   = 0x01;
}

void gs_packet_die_pack(packet_t *dest, struct gs_packet_die *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);
        assert(src->obj_id);

        type = 0x06;

        packet_append_val(dest, type);
        packet_append_val(dest, src->obj_id);
        packet_append_val(dest, src->to_village);
        packet_append_val(dest, src->to_hideaway);
        packet_append_val(dest, src->to_castle);
        packet_append_val(dest, src->to_siege);
        packet_append_val(dest, src->sweepable);
        packet_append_val(dest, src->to_fixed);
}
