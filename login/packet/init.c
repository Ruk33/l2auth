#ifndef L2AUTH_LOGIN_PACKET_INIT_C
#define L2AUTH_LOGIN_PACKET_INIT_C

#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_rsa_key.h>
#include <core/l2_packet.h>
#include <login/packet/init.h>

l2_packet* login_packet_init(struct L2RSAKey *rsa_key)
{
        l2_packet_type type = 0x00;

        unsigned char session_id[] = {
                0xfd,
                0x8a,
                0x22,
                0x00,
        };

        unsigned char protocol[] = {
                0x5a,
                0x78,
                0x00,
                0x00,
        };

        int rsa_size = l2_rsa_key_size(rsa_key);

        unsigned short content_size = (unsigned short) (
                sizeof(session_id) +
                sizeof(protocol) +
                (size_t) (rsa_size)
        );

        unsigned char* content;
        l2_packet* packet;

        if (!rsa_key) {
                log_fatal("No rsa key was passed in login init packet");
                return NULL;
        }

        content = calloc(content_size, sizeof(char));

        memcpy(content, session_id, sizeof(session_id));
        memcpy(content + sizeof(session_id), protocol, sizeof(protocol));
        l2_rsa_key_modulus(rsa_key, content + sizeof(session_id) + sizeof(protocol));

        packet = l2_packet_new(
                type,
                content,
                content_size
        );

        free(content);

        return packet;
}

#endif
