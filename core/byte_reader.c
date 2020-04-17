#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/byte_reader.h>

void* byte_reader_copy_and_move
(
        void* src,
        void* dest, 
        size_t to_copy
)
{
        assert(src);
        assert(dest);
        memcpy(dest, src, to_copy);
        return src + to_copy;
}
