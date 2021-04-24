#include <character.h>
#include "character.h"

/**
 * Calculate and update all character's
 * that are in queue to move.
 * This function will also notify close
 * characters to be in sync.
 */
// static void update_characters_position(void)
// {
//         character_t characters[10];
//         character_t character;
//         position_t  new_position;
//         size_t      packet_move_size = 0;
//         session_t   session;

//         character_move_forward(&character);
//         server_packet_move(NULL, character, &new_position);

//         packet_move_size = (size_t) packet_get_size(NULL);
//         session_encrypt_packet(session, NULL, NULL, packet_move_size);
// }

void timer_character_tick(void)
{
        // update_characters_position();
}
