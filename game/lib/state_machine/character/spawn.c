#include <headers.h>
#include <request.h>
#include <character.h>
#include <storage/character.h>
#include <server_packet/char_info.h>
#include <server_packet/npc_info.h>
#include "spawn.h"

static void spawn_random_orc(request_t *request)
{
        size_t  packet_safe_size = 0;
        packet *npc_info_packet  = NULL;

        character_t orc = { 0 };

        assert_valid_request(request);

        packet_safe_size =
                PACKET_SAFE_FULL_SIZE(sizeof(server_packet_npc_info_t));
        npc_info_packet = request->host->alloc_memory(packet_safe_size);

        orc.id = rand();
        orc.x  = -84023;
        orc.y  = 244598;
        orc.z  = -3730;

        strcat(orc.name, "Orc");
        strcat(orc.title, "Archer");

        server_packet_npc_info(npc_info_packet, &orc);
        session_encrypt_packet(
                request->session,
                npc_info_packet,
                npc_info_packet,
                (size_t) packet_get_size(npc_info_packet));
        request->host->send_response(
                request->session->socket,
                npc_info_packet,
                (size_t) packet_get_size(npc_info_packet));

        request->host->dealloc_memory(npc_info_packet);
}

static void spawn_character(request_t *request, character_t *spawned_character)
{
        size_t       packet_safe_size       = 0;
        packet *     char_info_packet       = NULL;
        size_t       close_characters_count = 0;
        size_t       max_close_characters   = 0;
        character_t *close_characters       = NULL;
        character_t *close_character        = NULL;

        assert_valid_request(request);
        assert(spawned_character);

        packet_safe_size     = PACKET_SAFE_FULL_SIZE(server_packet_char_info_t);
        char_info_packet     = request->host->alloc_memory(packet_safe_size);
        max_close_characters = 10;
        close_characters     = request->host->alloc_memory(
                sizeof(*close_characters) * max_close_characters);
        close_characters_count = storage_character_close_to(
                &request->storage->character_storage,
                close_characters,
                max_close_characters,
                spawned_character,
                1200);

        printf("Player spawned\n");

        for (size_t i = 0; i < close_characters_count; i++) {
                close_character = &close_characters[i];

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
                        close_character->session,
                        char_info_packet,
                        char_info_packet,
                        (size_t) packet_get_size(char_info_packet));
                request->host->send_response(
                        close_character->session->socket,
                        char_info_packet,
                        (size_t) packet_get_size(char_info_packet));

                /*
                 * Also notify the spawning player of the close character.
                 */
                server_packet_char_info(char_info_packet, close_character, 0);
                session_encrypt_packet(
                        request->session,
                        char_info_packet,
                        char_info_packet,
                        (size_t) packet_get_size(char_info_packet));
                request->host->send_response(
                        request->session->socket,
                        char_info_packet,
                        (size_t) packet_get_size(char_info_packet));
        }

        spawn_random_orc(request);

        request->host->dealloc_memory(char_info_packet);
        request->host->dealloc_memory(close_characters);
}

void state_machine_character_spawn(request_t *request, character_t *character)
{
        assert_valid_request(request);
        assert(character);
        spawn_character(request, character);
        character_update_state(character, IDLE);
}
