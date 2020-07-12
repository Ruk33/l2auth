#include <assert.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include <game/client.h>
#include "response.h"

void append_char
(
        byte_builder* buffer,
        unsigned int race,
        unsigned int class,
        unsigned int str,
        unsigned int dex,
        unsigned int con,
        unsigned int _int,
        unsigned int wit,
        unsigned int men
)
{
        unsigned int separator_46 = 0x46;
        unsigned int separator_0a = 0x0a;

        byte_builder_append(buffer, (unsigned char *) &race, sizeof(race)); 
        byte_builder_append(buffer, (unsigned char *) &class, sizeof(class));
        byte_builder_append(buffer, (unsigned char *) &separator_46, sizeof(separator_46));

        byte_builder_append(buffer, (unsigned char *) &str, sizeof(str));
        byte_builder_append(buffer, (unsigned char *) &separator_0a, sizeof(separator_0a));
        byte_builder_append(buffer, (unsigned char *) &separator_46, sizeof(separator_46));

        byte_builder_append(buffer, (unsigned char *) &dex, sizeof(dex));
        byte_builder_append(buffer, (unsigned char *) &separator_0a, sizeof(separator_0a));
        byte_builder_append(buffer, (unsigned char *) &separator_46, sizeof(separator_46));

        byte_builder_append(buffer, (unsigned char *) &con, sizeof(con));
        byte_builder_append(buffer, (unsigned char *) &separator_0a, sizeof(separator_0a));
        byte_builder_append(buffer, (unsigned char *) &separator_46, sizeof(separator_46));

        byte_builder_append(buffer, (unsigned char *) &_int, sizeof(_int));
        byte_builder_append(buffer, (unsigned char *) &separator_0a, sizeof(separator_0a));
        byte_builder_append(buffer, (unsigned char *) &separator_46, sizeof(separator_46));

        byte_builder_append(buffer, (unsigned char *) &wit, sizeof(wit));
        byte_builder_append(buffer, (unsigned char *) &separator_0a, sizeof(separator_0a));
        byte_builder_append(buffer, (unsigned char *) &separator_46, sizeof(separator_46));

        byte_builder_append(buffer, (unsigned char *) &men, sizeof(men));
        byte_builder_append(buffer, (unsigned char *) &separator_0a, sizeof(separator_0a));
}

l2_packet* game_request_new_character_response(struct GameClient* client)
{
        assert(client);

        l2_packet_type type = 0x17;
        byte_builder* buffer = game_client_byte_builder(client, 1000);
        unsigned int chars = 10;

        byte_builder_append(buffer, (unsigned char *) &chars, sizeof(chars));

        append_char(buffer, 0, 0, /*str*/ 40, /* dex */ 30, /* con */ 43, /* int */ 21, /* wit */ 11, /* men */ 25); // human fighter
        append_char(buffer, 0, 0, /*str*/ 40, /* dex */ 30, /* con */ 43, /* int */ 21, /* wit */ 11, /* men */ 25); // human fighter
        append_char(buffer, 0, 10, /*str*/ 22, /* dex */ 21, /* con */ 27, /* int */ 41, /* wit */ 20, /* men */ 39); // human mage
        
        append_char(buffer, 1, 18, /*str*/ 36, /* dex */ 35, /* con */ 36, /* int */ 23, /* wit */ 14, /* men */ 26); // elf fighter
        append_char(buffer, 1, 25, /*str*/ 21, /* dex */ 24, /* con */ 25, /* int */ 37, /* wit */ 23, /* men */ 40); // elf mage
        
        append_char(buffer, 2, 31, /*str*/ 41, /* dex */ 34, /* con */ 32, /* int */ 25, /* wit */ 12, /* men */ 26); // dark elf fighter
        append_char(buffer, 2, 38, /*str*/ 23, /* dex */ 23, /* con */ 24, /* int */ 44, /* wit */ 19, /* men */ 37); // dark elf mage
        
        append_char(buffer, 3, 44, /*str*/ 40, /* dex */ 26, /* con */ 47, /* int */ 18, /* wit */ 12, /* men */ 27); // orc fighter
        append_char(buffer, 3, 49, /*str*/ 27, /* dex */ 24, /* con */ 31, /* int */ 31, /* wit */ 15, /* men */ 42); // orc mage
        
        append_char(buffer, 4, 53, /*str*/ 39, /* dex */ 29, /* con */ 45, /* int */ 20, /* wit */ 10, /* men */ 27); // dwarf

        return game_client_create_packet(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );
}
