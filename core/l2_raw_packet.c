#ifndef L2AUTH_RAW_PACKET_C
#define L2AUTH_RAW_PACKET_C

#include <stdlib.h>
#include <string.h>

typedef unsigned char l2_raw_packet;

void l2_raw_packet_init(
        l2_raw_packet* packet,
        unsigned char* content,
        unsigned short content_size
)
{
        unsigned short packet_size = sizeof(unsigned short) + content_size;

        if (!packet || !content)
                return;

        memcpy(packet, &packet_size, sizeof(packet_size));
        memcpy(packet + sizeof(packet_size), content, content_size);
}

unsigned short l2_raw_packet_calc_required_mem(unsigned short content_size)
{
        return sizeof(unsigned short) + content_size;
}

l2_raw_packet* l2_raw_packet_new(
        unsigned char* content,
        unsigned short content_size
)
{
        l2_raw_packet* packet = calloc(
                l2_raw_packet_calc_required_mem(content_size),
                sizeof(l2_raw_packet)
        );

        l2_raw_packet_init(packet, content, content_size);

        return packet;
}

unsigned short l2_raw_packet_get_size(l2_raw_packet* packet)
{
        unsigned short size = 0;

        if (packet)
                memcpy(&size, packet, sizeof(size));

        return size;
}

void l2_raw_packet_content(
        l2_raw_packet* packet,
        unsigned char* dest,
        unsigned short start,
        unsigned short end
)
{
        if (!packet || !dest)
                return;
        
        memcpy(dest, packet + sizeof(unsigned short) + start, end);
}

#endif //L2AUTH_RAW_PACKET_C
