#include <unistd.h>
#include "timer.h"

void timer_start(unsigned int seconds, timer_callback callback)
{
        while (1) {
                callback();
                sleep(seconds);
        }
}
