#ifndef L2AUTH_GAME_HANDLER_AUTH_LOGIN_C
#define L2AUTH_GAME_HANDLER_AUTH_LOGIN_C

#include <string.h>
#include <log/log.h>
#include <core/l2_packet.c>
#include <game/packet/player_auth_request.c>
#include <login/dto/session_key.c>
#include <game/crypt.c>
#include <game/packet/char_list.c>

void copy_account_name_in(char* dest, unsigned char* src, size_t len)
{
        int i = 0;
        int c = 0;
        while (len--) {
                dest[c] = src[i];
                i += 2; // l2 string are 2 bytes wide (1 byte = char, 2 => char code in table)
                c++;
        }
}

void debug_packet(l2_raw_packet* packet)
{
        for (int i = 2, max = l2_raw_packet_get_size(packet); i < max; i++) {
                log_info("%02X | %c", packet[i], packet[i]);
        }
}

l2_packet* game_handler_auth_login(l2_raw_packet* request)
{
        unsigned char encrypt_key[] = {
                0x94,
                0x35,
                0x00,
                0x00,
                0xa1,
                0x6c,
                0x54,
                0x87,
        };
        l2_packet* response = game_packet_char_list();

        log_info("Debug response packet before encrypt");
        debug_packet(response);

        game_crypt_checksum(response + 2, l2_packet_get_content_size(response));
        game_crypt_encrypt(response + 2, l2_raw_packet_get_size(response), encrypt_key);

        log_info("Debug response packet after encrypt");
        debug_packet(response);

        return response;
}

#endif
