#include <assert.h>
#include <stdlib.h>
#include <core/l2_packet.h>
#include <core/byte_builder.h>
#include "../../client.h"
#include "../../packet_builder.h"
#include "response.h"

l2_packet *logout_response
(struct Client *client)
{
        assert(client);

        l2_packet_type type = 0x7e;
        l2_packet *response = NULL;

        unsigned char buffer[] = { 0, 0, 0, 0 };

        response = packet_builder_new(
                client,
                type,
                buffer,
                (unsigned short) sizeof(buffer)
        );

        return response;
}
