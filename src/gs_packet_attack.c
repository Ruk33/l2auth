#include <assert.h>
#include "include/util.h"
#include "include/packet.h"
#include "include/gs_types.h"
#include "include/gs_packet_attack.h"

void gs_packet_attack_set_attacker(
        gs_packet_attack_t *dest,
        struct gs_character *src)
{
        assert(dest);
        assert(src);

        dest->attacker_id = src->id;
        dest->attacker_x  = src->position.x;
        dest->attacker_y  = src->position.y;
        dest->attacker_z  = src->position.z;
}

void gs_packet_attack_add_hit(
        gs_packet_attack_t *dest,
        gs_packet_attack_hit_t *src)
{
        assert(dest);
        assert(src);

        // Todo: make sure we don't overflow
        dest->hits[dest->hit_count] = *src;
        dest->hit_count += 1;
}

void gs_packet_attack_pack(packet_t *dest, gs_packet_attack_t *src)
{
        byte_t type = 0x00;

        u16_t updated_hit_count = 0;

        assert(dest);
        assert(src);

        type = 0x05;

        updated_hit_count = src->hit_count - 1;

        packet_append_val(dest, type);
        packet_append_val(dest, src->attacker_id);
        packet_append_val(dest, src->hits[0].target_id);
        packet_append_val(dest, src->hits[0].damage);
        packet_append_val(dest, src->hits[0].flags);
        packet_append_val(dest, src->attacker_x);
        packet_append_val(dest, src->attacker_y);
        packet_append_val(dest, src->attacker_z);
        packet_append_val(dest, updated_hit_count);

        for (u16_t i = 1; i < src->hit_count; i += 1) {
                packet_append_val(dest, src->hits[i].target_id);
                packet_append_val(dest, src->hits[i].damage);
                packet_append_val(dest, src->hits[i].flags);
        }
}
