#ifndef L2AUTH_PACKET_C
#define L2AUTH_PACKET_C

#include <stdlib.h>
#include <string.h>
#include <core/l2_raw_packet.c>

typedef l2_raw_packet l2_packet;
typedef unsigned char l2_packet_type;

void l2_packet_init(
        l2_packet* packet,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
)
{
        l2_raw_packet_size packet_size = (l2_raw_packet_size) (sizeof(l2_packet_type) + content_size);
        unsigned char packet_content[packet_size];

        if (!packet || !content)
                return;
        
        memcpy(packet_content, &type, sizeof(type));
        memcpy(packet_content + sizeof(type), content, content_size);

        l2_raw_packet_init(packet, packet_content, packet_size);
}

l2_raw_packet_size l2_packet_calculate_size(unsigned short content_size)
{
        return (l2_raw_packet_size) (
                l2_raw_packet_calculate_size(content_size) +
                sizeof(l2_packet_type)
        );
}

l2_packet* l2_packet_new(
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
)
{
        l2_packet* packet = calloc(
                l2_packet_calculate_size(content_size),
                sizeof(l2_packet)
        );

        l2_packet_init(packet, type, content, content_size);

        return packet;
}

l2_packet_type l2_packet_get_type(l2_packet* packet)
{
        l2_packet_type type = 0;
        l2_raw_packet_content(packet, &type, 0, sizeof(type));
        return type;
}

void l2_packet_content(
        l2_packet* packet,
        unsigned char* dest,
        unsigned short start,
        unsigned short end
)
{
        l2_raw_packet_content(
                packet,
                dest,
                (unsigned short) (sizeof(l2_packet_type) + start),
                end
        );
}

#endif //L2AUTH_PACKET_C
