#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <core/byte_reader.h>

void* byte_reader_cpy_n_mv
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

void* byte_reader_cpy_int_n_mv(void* src, int* dest)
{
        return byte_reader_cpy_n_mv(src, dest, sizeof(int));
}

void* byte_reader_cpy_short_n_mv(void* src, short* dest)
{
        return byte_reader_cpy_n_mv(src, dest, sizeof(short));
}

void* byte_reader_cpy_char_n_mv(void* src, char* dest)
{
        return byte_reader_cpy_n_mv(src, dest, sizeof(char));
}
