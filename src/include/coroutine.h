#ifndef COROUTINE_H
#define COROUTINE_H

// all credit goes to https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define coroutine_begin(ctx)                \
    do {                                    \
        struct coroutine *tmp_ctx = (ctx);  \
        switch(tmp_ctx->state) {            \
        case 0:

#define yield(id)               \
    do {                        \
        tmp_ctx->state = (id);  \
        return;                 \
        case (id):;             \
    } while (0)

#define yield1 yield(1)
#define yield2 yield(2)
#define yield3 yield(3)
#define yield4 yield(4)
#define yield5 yield(5)
#define yield6 yield(6)
#define yield7 yield(7)

#define coroutine_end \
        }             \
    } while (0)

struct coroutine {
    float timeout;
    int state;
};

#endif