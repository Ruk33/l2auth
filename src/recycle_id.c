#include <assert.h>
#include "include/util.h"
#include "include/recycle_id.h"

int recycle_id_get(size_t *dest, size_t *instances)
{
        assert(instances);

        *dest = instances[0];

        if (instances[*dest]) {
                instances[0] = instances[*dest];
                return 0;
        }

        instances[0] = *dest + 1;
        return 1;
}

void recycle_id(size_t *instances, size_t id)
{
        assert(instances);
        instances[id] = instances[0];
        instances[0]  = id;
}
