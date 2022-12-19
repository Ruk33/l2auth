#include <stdlib.h> // random
#include <time.h>   // time
#include "include/l2auth.h"

// credits to https://www.youtube.com/watch?v=443UNeGrFoM
u32 l2_random(void)
{
    // just a simple thing to get something working.
    static u32 seed = 0;
    if (!seed)
        seed = time(0);
    seed = (seed << 13) ^ seed;
    u32 result = ((seed * (seed * seed * 15731 + 789221) + 1376312589) & 0x7fffffff);
    return result;
}

u32 l2_random_between(u32 min, u32 max)
{
    assert(min <= max);
    u32 result = random();
    result = result * (max - min + 1) + min;
    return result;
}
