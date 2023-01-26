#ifndef COROUTINE_H
#define COROUTINE_H

// all credit goes to https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define coroutine_begin(ctx)                \
    {                                       \
        struct coroutine *tmp_ctx = (ctx);  \
        switch(tmp_ctx->state) {            \
        case 0:

#define yield1                      \
    do {                            \
        tmp_ctx->state = 1;         \
        return;                     \
        case 1:;                    \
    } while (0)

#define yield2                      \
    do {                            \
        tmp_ctx->state = 2;         \
        return;                     \
        case 2:;                    \
    } while (0)

#define yield3                      \
    do {                            \
        tmp_ctx->state = 3;         \
        return;                     \
        case 3:;                    \
    } while (0)

#define yield4                      \
    do {                            \
        tmp_ctx->state = 4;         \
        return;                     \
        case 4:;                    \
    } while (0)

#define yield5                      \
    do {                            \
        tmp_ctx->state = 5;         \
        return;                     \
        case 5:;                    \
    } while (0)

#define yield6                      \
    do {                            \
        tmp_ctx->state = 6;         \
        return;                     \
        case 6:;                    \
    } while (0)

#define yield7                      \
    do {                            \
        tmp_ctx->state = 7;         \
        return;                     \
        case 7:;                    \
    } while (0)

#define coroutine_end   \
        }               \
    }

struct coroutine {
    float timeout;
    int state;
};

#endif