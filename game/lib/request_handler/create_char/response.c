#include <assert.h>
#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *create_char_response(struct Client *client)
{
        assert(client);

        l2_packet *response = NULL;

        l2_packet_type type = 0x19;
        int content = 0x01;

        response = packet_builder_new(
            client,
            type,
            (unsigned char *)&content,
            (unsigned short)sizeof(content));

        return response;
}
