#ifndef L2AUTH_SERVER_PACKET_INIT_C
#define L2AUTH_SERVER_PACKET_INIT_C

#include <core/l2_rsa_key.c>
#include <core/l2_packet.c>

l2_packet* server_packet_init(struct l2_rsa_key *rsa_key)
{
        l2_packet_type type = 0x00;

        unsigned char session_id[] = {
                (char) 0xfd,
                (char) 0x8a,
                (char) 0x22,
                (char) 0x00,
        };

        unsigned char protocol[] = {
                (char) 0x5a,
                (char) 0x78,
                (char) 0x00,
                (char) 0x00,
        };

        int rsa_size = l2_rsa_key_size(rsa_key);

        unsigned short content_size = (
                sizeof(session_id) +
                sizeof(protocol) +
                rsa_size
        );

        unsigned char content[content_size];

        memcpy(content, session_id, sizeof(session_id));
        memcpy(content + sizeof(session_id), protocol, sizeof(protocol));
        l2_rsa_key_modulus(rsa_key, content + sizeof(session_id) + sizeof(protocol));

        return l2_packet_new(
                type,
                content,
                content_size
        );
}

#endif //L2AUTH_SERVER_PACKET_INIT_C
