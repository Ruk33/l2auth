#include <headers.h>
#include <character.h>
#include "validate_position.h"

void server_packet_validate_position(
        packet *     dest,
        character_t *character,
        position_t * position,
        int          heading)
{
        assert(dest);
        assert(character);

        packet_type type = 0x61;

        byte_t  buf[sizeof(server_packet_validate_position_t)] = { 0 };
        byte_t *p                                              = buf;

        server_packet_validate_position_t validate_position_packet = { 0 };

        validate_position_packet.obj_id  = character->id;
        validate_position_packet.x       = position->x;
        validate_position_packet.y       = position->y;
        validate_position_packet.z       = position->z;
        validate_position_packet.heading = heading;

        BYTE_WRITE_VAL(p, validate_position_packet.obj_id);
        BYTE_WRITE_VAL(p, validate_position_packet.x);
        BYTE_WRITE_VAL(p, validate_position_packet.y);
        BYTE_WRITE_VAL(p, validate_position_packet.z);
        BYTE_WRITE_VAL(p, validate_position_packet.heading);

        packet_build(dest, type, buf, (size_t)(p - buf));
}
