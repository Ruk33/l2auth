#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>

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
        assert(packet);
        assert(content);
        assert(content_size);

        l2_raw_packet_size size_header = l2_raw_packet_calculate_size(
                content_size
        );

        memcpy(packet, &size_header, sizeof(size_header));
        memcpy(packet + sizeof(size_header), content, content_size);
}

l2_raw_packet_size l2_raw_packet_get_size(l2_raw_packet* packet)
{
        assert(packet);
        l2_raw_packet_size size = 0;
        memcpy(&size, packet, sizeof(size));
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
        assert(packet);
        assert(dest);
        
        memcpy(
                dest,
                packet + sizeof(l2_raw_packet_size) + start,
                end
        );
}
