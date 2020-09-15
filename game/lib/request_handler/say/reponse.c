#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/byte_builder.h>
#include <core/l2_packet.h>
#include <core/l2_string.h>
#include "../../client.h"
#include "../../entity/pc.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *say_response
(struct Client *client, l2_string* message)
{
        assert(client);
        assert(message);

        l2_packet_type type = 0x4a;
        l2_packet *response = NULL;

        struct Pc *player = client_player(client);
        int text_type = 0;

        // char new_char_message[] = "Like do you even bro?";
        // l2_string *message = client_alloc_mem(
        //         client,
        //         l2_string_calculate_space_from_char(sizeof(new_char_message))
        // );
        // l2_string_from_char(message, new_char_message, sizeof(new_char_message));

        size_t message_length = l2_string_len(message) + 1;
        size_t message_as_string_length = l2_string_calculate_space_from_char(
                message_length
        );

        size_t name_len = strlen(player->character.name) + 1;
        size_t name_as_string_len = l2_string_calculate_space_from_char(name_len);
        l2_string *name_as_string = client_alloc_mem(client, name_as_string_len);

        size_t buf_size = byte_builder_calculate_size(
                sizeof(player->character.id) +
                sizeof(text_type) +
                name_as_string_len +
                message_as_string_length
        );
        byte_builder *buf = client_alloc_mem(client, buf_size);
        byte_builder *buffer = byte_builder_init(buf, buf_size);

        l2_string_from_char(name_as_string, player->character.name, name_len);

        byte_builder_append_int(buffer, &player->character.id);
        byte_builder_append_int(buffer, &text_type);
        byte_builder_append(buffer, name_as_string, name_as_string_len);
        byte_builder_append(buffer, message, message_as_string_length);

        response = packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );

        client_free_mem(client, player);
        client_free_mem(client, name_as_string);
        client_free_mem(client, buf);

        return response;
}
