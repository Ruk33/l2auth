#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"

void packet_build(unsigned char *dest, unsigned char type, unsigned char *content, size_t content_size)
{
        assert(dest);
        // Packet of type 0 is also valid
        // assert(type);
        assert(content);
        assert(content_size);

        unsigned short size = (unsigned short) (sizeof(size) + sizeof(type) + content_size);
        unsigned char *packet_size = dest;
        unsigned char *packet_type = packet_size + sizeof(size);
        unsigned char *packet_content = packet_type + sizeof(type);

        memcpy(packet_size, &size, sizeof(size));
        memcpy(packet_type, &type, sizeof(type));
        memcpy(packet_content, content, content_size);
}

unsigned short packet_size(unsigned char *packet)
{
        assert(packet);
        unsigned short size = 0;
        memcpy(&size, packet, sizeof(size));
        return size;
}

unsigned char packet_type(unsigned char *packet)
{
        assert(packet);
        return packet[2];
}

unsigned char *packet_body(unsigned char *packet)
{
        assert(packet);
        size_t size_header = sizeof(short);
        size_t type_header = sizeof(char);
        return packet + size_header + type_header;
}
