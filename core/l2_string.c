#include <assert.h>
#include <stdlib.h>
#include <core/l2_string.h>

void l2_string_to_char(l2_string* src, char* dest, size_t size)
{
        assert(src);
        assert(dest);

        for(size_t i = 0; i < size; i++) {
                dest[i] = *src;
                src += 2;
        }
}

void l2_string_from_char(l2_string* dest, char* src, size_t size)
{
        assert(dest);
        assert(src);

        for(size_t i = 0; i < size; i++) {
                *dest = src[i];
                *(dest + 1) = 0;
                dest += 2;
        }
}

void l2_string_from_l2(l2_string* dest, l2_string* src, size_t size)
{
        assert(dest);
        assert(src);

        for(size_t i = 0, max = size * 2; i < max; i += 2) {
                dest[i] = src[i];
                dest[i + 1] = src[i + 1];
        }
}

size_t l2_string_calculate_space_from_char(size_t original_string)
{
        return original_string * 2;
}

size_t l2_string_len(l2_string* src)
{
        assert(src);
        l2_string* c = src;
        size_t length = 0;
        while (*c != 0) {
                length++;
                c += 2;
        }
        return length;
}
