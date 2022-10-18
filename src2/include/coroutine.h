#ifndef COROUTINE_H
#define COROUTINE_H

// all credit goes to https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define ctx_begin(ctx) \
    switch((ctx)->state) { \
    case 0:

#define ctx_return(ctx, x) \
    do { \
        (ctx)->state = __LINE__; \
        return(x); \
        case __LINE__:; \
    } while (0)

#define ctx_yield(ctx) \
    do { \
        (ctx)->state = __LINE__; \
        return; \
        case __LINE__:; \
    } while (0)

#define ctx_end() \
    }

struct ctx {
    int state;
};

#endif