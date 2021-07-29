#include <assert.h>
#include "include/util.h"
#include "include/recycle_id.h"

int recycle_id_get(size_t *dest, size_t *src)
{
        assert(src);

        *dest = src[0];

        if (src[*dest]) {
                src[0] = src[*dest];
                return 0;
        }

        src[0] = *dest + 1;
        return 1;
}

void recycle_id(size_t *src, size_t id)
{
        assert(src);
        src[id] = src[0];
        src[0]  = id;
}
