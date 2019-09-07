#include <string.h>

typedef struct packet_builder {
        unsigned char type;
        void *content;
        size_t content_size;
} packet_builder;

void packet_builder_set_type(
        packet_builder* builder,
        unsigned char type
)
{
        if (!builder)
                return;

        builder->type = type;
}

void packet_builder_set_content(
        packet_builder *builder,
        void* content,
        size_t size
)
{
       builder->content = content;
       builder->content_size = size;
}

void packet_builder_build(
        packet_builder* builder,
        void* dest,
        size_t dest_size
)
{
        unsigned short packet_size;

        if (!builder || !dest)
                return;
        
        packet_size = (unsigned short)
                sizeof(builder->type) + builder->content_size;

        memcpy(dest, &packet_size, sizeof(packet_size));

        memcpy(
                dest + sizeof(packet_size),
                &builder->type,
                sizeof(builder->type)
        );

        memcpy(
                dest
                + sizeof(packet_size)
                + sizeof(builder->type),
                builder->content,
                builder->content_size
        );
}
