#include <stdlib.h>
#include <time.h>
#include "include/random.h"

u32 random_u32(void)
{
    srand(time(0));
    u32 result = rand();
    return result;
}
