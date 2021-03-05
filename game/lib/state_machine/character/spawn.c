#include <headers.h>
#include <request.h>
#include <character.h>
#include <storage/character.h>
#include <server_packet/char_info.h>
#include "spawn.h"

static void spawn_character(request_t *request, character_t *spawned_character)
{
        packet *char_info_packet = NULL;

        struct List *close_characters = NULL;
        struct ListEntry *i_close_character = NULL;
        character_t *close_character = NULL;

        assert_valid_request(request);
        assert(spawned_character);

        char_info_packet = request->host->alloc_memory(
                PACKET_SAFE_FULL_SIZE(server_packet_char_info_t));
        close_characters = list_create(request->host->alloc_memory,
                                       request->host->dealloc_memory);
        storage_character_close_to(&request->storage->character_storage,
                                   &close_characters, spawned_character, 1200);
        i_close_character = list_get_iterator(close_characters);

        printf("Player spawned\n");

        while (i_close_character) {
                close_character = list_get_value(i_close_character);
                i_close_character = list_get_next(i_close_character);

                printf("Showing new spawned player to %d\n",
                       close_character->id);

                if (close_character->id == spawned_character->id) {
                        printf("Same character, ignoring\n");
                        continue;
                }

                /*
                 * Notify close character of the player being spawn.
                 */
                server_packet_char_info(char_info_packet, spawned_character, 0);
                session_encrypt_packet(
                        close_character->session, char_info_packet,
                        char_info_packet,
                        (size_t) packet_get_size(char_info_packet));
                request->host->send_response(
                        close_character->session->socket, char_info_packet,
                        (size_t) packet_get_size(char_info_packet));

                /*
                 * Also notify the spawning player of the close character.
                 */
                server_packet_char_info(char_info_packet, close_character, 0);
                session_encrypt_packet(
                        request->session, char_info_packet, char_info_packet,
                        (size_t) packet_get_size(char_info_packet));
                request->host->send_response(
                        request->session->socket, char_info_packet,
                        (size_t) packet_get_size(char_info_packet));
        }

        request->host->dealloc_memory(char_info_packet);
        list_free(close_characters);
}

void state_machine_character_spawn(request_t *request, character_t *character)
{
        assert_valid_request(request);
        assert(character);
        spawn_character(request, character);
        character_update_state(character, IDLE);
}
