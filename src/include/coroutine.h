#ifndef COROUTINE_H
#define COROUTINE_H

struct coroutine {
    float timeout;
    int state;
};

// all credit goes to 
// https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
#define coroutine(ctx)                   \
    struct coroutine *__coro = (ctx);    \
    switch (__coro->state)               \
    case 0:

#define yield(id)               \
    do {                        \
        __coro->state = (id);  \
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

#endif
