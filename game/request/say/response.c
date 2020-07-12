#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <core/l2_packet.h>
#include <core/l2_string.h>
#include <core/byte_builder.h>
#include <game/client.h>
#include <game/entity/character.h>
#include "response.h"

l2_packet* game_request_say_response
(
        struct GameClient *client,
        l2_string* message
)
{
        assert(client);
        assert(message);

        l2_packet_type type = 0x4a;
        int text_type = 0;
        struct GameEntityCharacter *character = game_client_get_char(client);

        size_t message_length = l2_string_len(message) + 1;
        size_t message_as_string_length =
                l2_string_calculate_space_from_char(message_length);

        size_t name_len = strlen(character->name) + 1;
        size_t name_as_string_len =
                l2_string_calculate_space_from_char(name_len);
        l2_string* name_as_string =
                game_client_alloc_temp_mem(client, name_as_string_len);
        assert(name_as_string);

        byte_builder* buffer = game_client_byte_builder(
                client,
                (
                        sizeof(character->char_id) +
                        sizeof(text_type) +
                        name_as_string_len +
                        message_as_string_length
                )
        );
        assert(buffer);

        l2_string_from_char(
                name_as_string,
                character->name,
                name_len
        );

        byte_builder_append(
                buffer,
                &character->char_id,
                sizeof(character->char_id)
        );
        byte_builder_append(buffer, &text_type, sizeof(text_type));
        byte_builder_append(buffer, name_as_string, name_as_string_len);
        byte_builder_append(buffer, message, message_as_string_length);

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
