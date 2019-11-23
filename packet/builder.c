#include <string.h>

void packet_builder_set_type
(
        void* packet,
        unsigned char type
)
{
        if (!packet) return;
        memcpy(sizeof(unsigned short) + packet, &type, sizeof(type));
}

void packet_builder_set_content
(
        void* packet,
        void* content,
        size_t size
)
{
        if (!packet || !content) return;
        memcpy(
                sizeof(unsigned short) + sizeof(unsigned char) + packet,
                content,
                size
        );
}
