#include <assert.h>
#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

static void fill_race_template
(byte_builder* buffer, int race, int class, int str, int dex, int con, int _int, int wit, int men)
{
        assert(buffer);

        int separator_46 = 0x46;
        int separator_0a = 0x0a;

        byte_builder_append_int(buffer, &race);
        byte_builder_append_int(buffer, &class);
        byte_builder_append_int(buffer, &separator_46);

        byte_builder_append_int(buffer, &str);
        byte_builder_append_int(buffer, &separator_0a);
        byte_builder_append_int(buffer, &separator_46);

        byte_builder_append_int(buffer, &dex);
        byte_builder_append_int(buffer, &separator_0a);
        byte_builder_append_int(buffer, &separator_46);

        byte_builder_append_int(buffer, &con);
        byte_builder_append_int(buffer, &separator_0a);
        byte_builder_append_int(buffer, &separator_46);

        byte_builder_append_int(buffer, &_int);
        byte_builder_append_int(buffer, &separator_0a);
        byte_builder_append_int(buffer, &separator_46);
        
        byte_builder_append_int(buffer, &wit);
        byte_builder_append_int(buffer, &separator_0a);
        byte_builder_append_int(buffer, &separator_46);

        byte_builder_append_int(buffer, &men);
        byte_builder_append_int(buffer, &separator_0a);
}

l2_packet *new_char_response
(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x17;
        l2_packet *response = NULL;

        size_t buf_size = 0;
        byte_builder *buf = NULL;
        byte_builder *buffer = NULL;

        int chars = 0;

        // We may not need this amount of mem but...
        buf_size = byte_builder_calculate_size(1024);
        buf = client_alloc_mem(client, buf_size);
        buffer = byte_builder_init(buf, buf_size);

        chars = 10;

        byte_builder_append_int(buffer, &chars);

        fill_race_template(buffer, 0, 0, /*str*/ 40, /* dex */ 30, /* con */ 43, /* int */ 21, /* wit */ 11, /* men */ 25); // human fighter
        fill_race_template(buffer, 0, 0, /*str*/ 40, /* dex */ 30, /* con */ 43, /* int */ 21, /* wit */ 11, /* men */ 25); // human fighter
        fill_race_template(buffer, 0, 10, /*str*/ 22, /* dex */ 21, /* con */ 27, /* int */ 41, /* wit */ 20, /* men */ 39); // human mage
        
        fill_race_template(buffer, 1, 18, /*str*/ 36, /* dex */ 35, /* con */ 36, /* int */ 23, /* wit */ 14, /* men */ 26); // elf fighter
        fill_race_template(buffer, 1, 25, /*str*/ 21, /* dex */ 24, /* con */ 25, /* int */ 37, /* wit */ 23, /* men */ 40); // elf mage
        
        fill_race_template(buffer, 2, 31, /*str*/ 41, /* dex */ 34, /* con */ 32, /* int */ 25, /* wit */ 12, /* men */ 26); // dark elf fighter
        fill_race_template(buffer, 2, 38, /*str*/ 23, /* dex */ 23, /* con */ 24, /* int */ 44, /* wit */ 19, /* men */ 37); // dark elf mage
        
        fill_race_template(buffer, 3, 44, /*str*/ 40, /* dex */ 26, /* con */ 47, /* int */ 18, /* wit */ 12, /* men */ 27); // orc fighter
        fill_race_template(buffer, 3, 49, /*str*/ 27, /* dex */ 24, /* con */ 31, /* int */ 31, /* wit */ 15, /* men */ 42); // orc mage
        
        fill_race_template(buffer, 4, 53, /*str*/ 39, /* dex */ 29, /* con */ 45, /* int */ 20, /* wit */ 10, /* men */ 27); // dwarf

        response = packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) byte_builder_length(buffer)
        );

        client_free_mem(client, buf);

        return response;
}
