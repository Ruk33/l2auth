#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_packet_status.h"

void gs_packet_status_add(
        struct gs_packet_status *dest,
        struct gs_packet_status_attr *src)
{
        assert(dest);
        assert(src);
        assert(dest->count < arr_size(dest->attributes));

        dest->attributes[dest->count] = *src;
        dest->count += 1;
}

void gs_packet_status_pack(packet_t *dest, struct gs_packet_status *src)
{
        byte_t type = 0x00;

        assert(dest);
        assert(src);
        assert(src->obj_id);
        assert(src->count);

        type = 0x0e;

        packet_append_val(dest, type);
        packet_append_val(dest, src->obj_id);
        packet_append_val(dest, src->count);

        for (u32_t i = 0; i < src->count; i += 1) {
                packet_append_val(dest, src->attributes[i].type);
                packet_append_val(dest, src->attributes[i].value);
        }
}
