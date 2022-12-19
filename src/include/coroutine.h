#ifndef COROUTINE_H
#define COROUTINE_H

#define nothing \
    ((void) 0)

// all credit goes to https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define coroutine_begin(ctx)                \
    {                                       \
        struct coroutine *tmp_ctx = (ctx);  \
        switch(tmp_ctx->state) {            \
        case 0:

#define yield_ex(x)                 \
    do {                            \
        tmp_ctx->state = __LINE__;  \
        return(x);                  \
        case __LINE__:;             \
    } while (0)

#define yield \
    yield_ex(nothing)

#define coroutine_end   \
        }               \
    }

#define yield_sleep_ex(_timeout, dt, x)     \
    do {                                    \
        (tmp_ctx)->timeout = (_timeout);    \
        yield_ex(x);                        \
        (tmp_ctx)->timeout -= (dt);         \
        if ((tmp_ctx)->timeout > 0)         \
            return(x);                      \
    } while(0)

#define yield_sleep(timeout, dt) \
    yield_sleep_ex((timeout), (dt), (nothing))

// this is error prone.
// let review it later in the future.
#define yield_while(condition)  \
    do {                        \
        if ((condition))        \
            yield;              \
        if ((condition))        \
            return;             \
    } while(0)

struct coroutine {
    float timeout;
    int state;
};

#endif