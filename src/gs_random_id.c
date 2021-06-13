#include <assert.h>
#include <openssl/rand.h>
#include "include/util.h"
#include "include/gs_random_id.h"

// Thanks to https://stackoverflow.com/a/31282121
int gs_random_id(u32_t *dest)
{
        union {
                u32_t i;
                byte_t c[sizeof(u32_t)];
        } u;

        u32_t limit = 65535;

        assert(dest);

        do {
                if (!RAND_bytes(u.c, sizeof(u.c))) {
                        return 0;
                }
        } while (u.i < (-limit % limit));

        *dest = u.i % limit;

        return 1;
}
