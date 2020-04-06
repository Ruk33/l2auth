#ifndef L2AUTH_PACKET_HELPER_C
#define L2AUTH_PACKET_HELPER_C

#include <stdlib.h>
#include <string.h>
#include <packet/helper.h>

unsigned short packet_helper_size(void* packet)
{
        unsigned short size = 0;
        if (packet) memcpy(&size, packet, sizeof(size));
        return size;
}

unsigned short packet_helper_full_size(void* packet)
{
        if (!packet) return 0;
        return (unsigned short) (
                sizeof(unsigned short) +
                packet_helper_size(packet)
        );
}

unsigned char packet_helper_type(void* packet)
{
        unsigned char type = 0;
        if (packet) memcpy(&type, sizeof(unsigned short) + packet, sizeof(type));
        return type;
}

void packet_helper_content
(
        void* packet,
        void* dest,
        size_t dest_size
)
{
        size_t size_header = sizeof(unsigned short);
        size_t type_header = sizeof(unsigned char);

        if (!packet || !dest) return;

        memcpy(
                dest,
                size_header + type_header + packet,
                dest_size
        );
}

#endif
