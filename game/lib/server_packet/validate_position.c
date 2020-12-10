#include "../headers.h"
#include "../character.h"
#include "validate_position.h"

void server_packet_validate_position(packet *dest, character_t *character, int heading)
{
        assert(dest);
        assert(character);

        packet_type type = 0x61;

        byte buf[SERVER_PACKET_VALIDATE_POSITION_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketValidatePosition validate_position_packet = {0};

        validate_position_packet.obj_id = character->id;
        validate_position_packet.x = character->x;
        validate_position_packet.y = character->y;
        validate_position_packet.z = character->z;
        validate_position_packet.heading = heading;

        BYTE_WRITE_VAL(p, validate_position_packet.obj_id);
        BYTE_WRITE_VAL(p, validate_position_packet.x);
        BYTE_WRITE_VAL(p, validate_position_packet.y);
        BYTE_WRITE_VAL(p, validate_position_packet.z);
        BYTE_WRITE_VAL(p, validate_position_packet.heading);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
