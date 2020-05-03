#include <stdlib.h>
#include <string.h>
#include <core/l2_packet.h>
#include <core/l2_client.h>
#include <core/l2_string.h>
#include <core/byte_builder.h>
#include <game/dto/char.h>
#include <game/packet/creature_say.h>

l2_packet* game_packet_creature_say
(
        struct L2Client *client,
        l2_string* message
)
{
        l2_packet_type type = 0x4a;
        int text_type = 0;
        struct GameDtoChar *character = l2_client_get_char(client);

        size_t message_length = l2_string_len(message) + 1;
        size_t message_as_string_length =
                l2_string_calculate_space_from_char(message_length);

        size_t name_len = strlen(character->name) + 1;
        size_t name_as_string_len =
                l2_string_calculate_space_from_char(name_len);
        l2_string* name_as_string =
                l2_client_alloc_temp_mem(client, name_as_string_len);

        byte_builder* buffer = l2_client_byte_builder(
                client,
                (
                        sizeof(character->char_id) +
                        sizeof(text_type) +
                        name_as_string_len +
                        message_as_string_length
                )
        );

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

        return l2_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
