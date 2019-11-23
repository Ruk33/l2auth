#ifndef L2AUTH_LOGIN_PACKET_OK_C
#define L2AUTH_LOGIN_PACKET_OK_C

#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_packet.c>
#include <login/session_key.c>

l2_packet* login_packet_ok(struct LoginSessionKey *session_key)
{
        l2_packet_type type = 0x03;
        size_t key1_size = session_key ? sizeof(session_key->loginOK1) : 0;
        size_t key2_size = session_key ? sizeof(session_key->loginOK2) : 0;
        unsigned char content_after_keys[] = {
                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0xea,
                0x03,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x02,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x00,
                0x00,
                0x00,
                0x00,

                0x60,
                0x62,
                0xe0,
                0x00,

                0x00,
                0x00,
                0x00,
        };
        size_t content_after_keys_size = sizeof(content_after_keys);
        size_t content_size = key1_size + key2_size + content_after_keys_size;
        unsigned char* content = calloc(content_size, sizeof(char));
        l2_packet* packet;

        if (session_key) {
                memcpy(content, &session_key->loginOK1, key1_size);
                memcpy(content + key1_size, &session_key->loginOK2, key2_size);
        } else {
                log_info("Warning, no session key was provided");
        }

        memcpy(content + key1_size + key2_size, &content_after_keys, sizeof(content_after_keys));

        packet = l2_packet_new(
                type,
                content,
                (unsigned short) content_size
        );

        free(content);

        return packet;
}

#endif
