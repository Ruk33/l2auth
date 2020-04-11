#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/l2_raw_packet.h>
#include <core/l2_packet.h>

void l2_packet_init
(
        l2_packet* packet,
        l2_packet_type type,
        unsigned char* content,
        unsigned short content_size
)
{
        assert(packet);
        assert(content);

        l2_raw_packet_size packet_size = (l2_raw_packet_size) (
                sizeof(l2_packet_type) +
                content_size
        );

        unsigned char* packet_content = calloc(packet_size, sizeof(char));

        memcpy(packet_content, &type, sizeof(type));
        memcpy(packet_content + sizeof(type), content, content_size);

        l2_raw_packet_init(packet, packet_content, packet_size);

        free(packet_content);
}

l2_raw_packet_size l2_packet_calculate_size(unsigned short content_size)
{
        return (l2_raw_packet_size) (
                l2_raw_packet_calculate_size(content_size) +
                sizeof(l2_packet_type)
        );
}

l2_packet_type l2_packet_get_type(l2_packet* packet)
{
        assert(packet);

        l2_packet_type type = 0;
        l2_raw_packet_content(packet, &type, 0, sizeof(type));

        return type;
}

unsigned short l2_packet_get_content_size(l2_packet* packet)
{
        assert(packet);
        return (unsigned short) (
                l2_raw_packet_get_size(packet) -
                sizeof(l2_raw_packet_size) -
                sizeof(l2_packet_type)
        );
}

void l2_packet_content
(
        l2_packet* packet,
        unsigned char* dest,
        unsigned short start,
        unsigned short end
)
{
        assert(packet);
        assert(dest);
        l2_raw_packet_content(
                packet,
                dest,
                (unsigned short) (sizeof(l2_packet_type) + start),
                end
        );
}
