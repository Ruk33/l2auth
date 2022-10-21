#ifndef COROUTINE_H
#define COROUTINE_H

// all credit goes to https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define co_begin(ctx) \
    { \
        struct co_ctx *tmp_ctx = (ctx); \
        switch(tmp_ctx->state) { \
        case 0:

#define yield_val(x) \
    do { \
        tmp_ctx->state = __LINE__; \
        return(x); \
        case __LINE__:; \
    } while (0)

#define yield \
    yield_val((void) 0)

#define co_end \
        } \
    }

#define co_sleep_val(timer, _timeout, dt, x) \
    do { \
        (timer)->timeout = (_timeout); \
        yield_val(x); \
        (timer)->timeout -= (dt); \
        if ((timer)->timeout > 0) \
            return(x); \
    } while(0)

#define co_sleep(timer, timeout, dt) \
    co_sleep_val((timer), (timeout), (dt), ((void) 0))

struct co_timer {
    float timeout;
};

struct co_ctx {
    int state;
};

#endif