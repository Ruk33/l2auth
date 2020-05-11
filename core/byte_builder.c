#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <log/log.h>
#include <core/byte_builder.h>

size_t byte_builder_calculate_size(size_t to_be_allocated)
{
        assert(to_be_allocated);

        size_t allocated_metadata = sizeof(size_t);
        size_t len_metadata = sizeof(size_t);

        return allocated_metadata + len_metadata + to_be_allocated;
}

byte_builder* byte_builder_init(byte_builder* builder, size_t allocated)
{
        assert(builder);
        assert(allocated);

        size_t allocated_metadata = sizeof(size_t);
        size_t len_metadata = sizeof(size_t);
        size_t length = 0;

        assert(allocated - allocated_metadata - len_metadata > 0);
        allocated -= allocated_metadata - len_metadata;

        memcpy(builder, &allocated, sizeof(allocated));
        memcpy(builder + allocated_metadata, &length, sizeof(length));

        return builder + allocated_metadata + len_metadata;
}

size_t byte_builder_allocated_mem(byte_builder* builder)
{
        assert(builder);

        size_t allocated = 0;
        size_t len_metadata = sizeof(size_t);

        memcpy(
                &allocated,
                builder - sizeof(allocated) - len_metadata,
                sizeof(allocated)
        );

        return allocated;
}

size_t byte_builder_length(byte_builder* builder)
{
        assert(builder);

        size_t length = 0;
        memcpy(&length, builder - sizeof(length), sizeof(length));

        return length;
}

void byte_builder_append
(
        byte_builder* builder,
        unsigned char* content,
        size_t content_size
)
{
        assert(builder);
        assert(content);

        size_t allocated_metadata = sizeof(size_t);
        size_t len = byte_builder_length(builder);
        size_t new_len = content_size + len;

        /*
        log_info(
                "Mem: %ld, Len: %ld, New len: %ld",
                byte_builder_allocated_mem(builder), 
                len,
                new_len
        );
        */

        assert(byte_builder_allocated_mem(builder) >= new_len);

        memcpy(builder + len, content, content_size);
        memcpy(builder - allocated_metadata, &new_len, sizeof(new_len));
}
