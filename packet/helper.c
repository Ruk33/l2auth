#ifndef L2AUTH_PACKET_HELPER_C
#define L2AUTH_PACKET_HELPER_C

#include <stdlib.h>
#include <string.h>

unsigned short packet_helper_size(void *packet)
{
        unsigned short size = 0;

        if (packet)
                memcpy(&size, packet, sizeof(packet));

        return size;
}

unsigned char packet_helper_type(void *packet)
{
        size_t size_header = sizeof(short);
        unsigned char type = 0;

        if (packet)
                memcpy(&type, size_header + packet, sizeof(type));

        return type;
}

void* packet_helper_content
(
        void* packet,
        void* dest,
        size_t dest_size
)
{
        size_t size_header = sizeof(unsigned short);
        size_t type_header = sizeof(unsigned char);

        if (!packet || !dest)
                return;

        memcpy(
                dest,
                size_header + type_header + packet,
                dest_size
        );
}

#endif
