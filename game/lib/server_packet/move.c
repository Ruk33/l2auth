#include "../headers.h"
#include "../character.h"
#include "move.h"

void server_packet_move(packet *dest, character_t *character, vec3_t *position)
{
        assert(dest);
        assert(character);
        assert(position);

        packet_type type = 0x01;

        byte buf[SERVER_PACKET_MOVE_SIZE] = {0};
        byte *p = buf;

        struct ServerPacketMove move_packet = {0};

        move_packet.obj_id = character->id;
        move_packet.new_position.x = position->x;
        move_packet.new_position.y = position->y;
        move_packet.new_position.z = position->z;
        move_packet.old_position.x = character->x;
        move_packet.old_position.y = character->y;
        move_packet.old_position.z = character->z;

        BYTE_WRITE_VAL(p, move_packet.obj_id);
        BYTE_WRITE_VAL(p, move_packet.new_position.x);
        BYTE_WRITE_VAL(p, move_packet.new_position.y);
        BYTE_WRITE_VAL(p, move_packet.new_position.z);
        BYTE_WRITE_VAL(p, move_packet.old_position.x);
        BYTE_WRITE_VAL(p, move_packet.old_position.y);
        BYTE_WRITE_VAL(p, move_packet.old_position.z);

        packet_build(dest, type, buf, (size_t) (p - buf));
}
