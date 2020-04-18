#ifndef L2AUTH_LOGIN_GAME_PACKET_CHAR_SELECTED_H
#define L2AUTH_LOGIN_GAME_PACKET_CHAR_SELECTED_H

#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <game/dto/char.h>

l2_packet* game_packet_char_selected
(
        struct L2Client* client,
        struct GameDtoChar* character
);

#endif
