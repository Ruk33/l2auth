#ifndef L2AUTH_RAW_PACKET_C
#define L2AUTH_RAW_PACKET_C

#include <stdlib.h>
#include <string.h>

typedef unsigned short l2_raw_packet_size;
typedef unsigned char l2_raw_packet;

l2_raw_packet_size l2_raw_packet_calculate_size
(
        unsigned short content_size
)
{
        return (l2_raw_packet_size) (
                sizeof(l2_raw_packet_size) +
                content_size
        );
}

void l2_raw_packet_init
(
        l2_raw_packet* packet,
        unsigned char* content,
        unsigned short content_size
)
{
        l2_raw_packet_size size_header = l2_raw_packet_calculate_size(content_size);

        if (!packet || !content) return;

        memcpy(packet, &size_header, sizeof(size_header));
        memcpy(packet + sizeof(size_header), content, content_size);
}

l2_raw_packet* l2_raw_packet_new
(
        unsigned char* content,
        unsigned short content_size
)
{
        l2_raw_packet* packet = calloc(
                l2_raw_packet_calculate_size(content_size),
                sizeof(l2_raw_packet)
        );

        l2_raw_packet_init(packet, content, content_size);

        return packet;
}

l2_raw_packet_size l2_raw_packet_get_size(l2_raw_packet* packet)
{
        l2_raw_packet_size size = 0;

        if (packet) memcpy(&size, packet, sizeof(size));

        return size;
}

void l2_raw_packet_content
(
        l2_raw_packet* packet,
        unsigned char* dest,
        unsigned short start,
        unsigned short end
)
{
        if (!packet || !dest) return;
        
        memcpy(
                dest,
                packet + sizeof(l2_raw_packet_size) + start,
                end
        );
}

void l2_raw_packet_free(l2_raw_packet* packet)
{
        if (packet) free(packet);
}

#endif
