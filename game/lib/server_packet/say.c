#include "../headers.h"
#include "../character.h"
#include "say.h"

void server_packet_say(
        packet *dest,
        character_t *character,
        enum ServerPacketSayMsgType msg_type,
        char *msg,
        size_t msg_size
)
{
        assert(dest);
        assert(character);
        assert(msg);
        assert(msg_size);

        packet_type type = 0x4a;

        unsigned char buf[SERVER_PACKET_SAY_SIZE] = {0};
        unsigned char *p = buf;

        struct ServerPacketSay say_packet = {0};
        size_t max_msg_size = sizeof(say_packet.msg) / 2;

        say_packet.sender_obj_id = character->id;
        say_packet.msg_type = msg_type;

        l2_string_from_char(
                say_packet.sender_name,
                character->name,
                strlen(character->name) + 1
        );

        l2_string_from_char(
                say_packet.msg,
                msg,
                msg_size < max_msg_size ? msg_size : max_msg_size
        );

        BYTE_WRITE_VAL(p, say_packet.sender_obj_id);
        BYTE_WRITE_VAL(p, say_packet.msg_type);
        BYTE_WRITE(p, say_packet.sender_name, l2_string_bytes(say_packet.sender_name));
        BYTE_WRITE(p, say_packet.msg, l2_string_bytes(say_packet.msg));

        packet_build(dest, type, buf, (size_t) (p - buf));
}
