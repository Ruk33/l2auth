#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

#include <stdio.h>

#define log(...)                     \
        {                            \
                printf(__VA_ARGS__); \
                printf("\n");        \
                fflush(stdout);      \
        }

#endif
