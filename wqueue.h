#include <windows.h>

struct wqueue;
typedef void (worker)(struct wqueue *q, void *work);

struct wqueue {
    CRITICAL_SECTION lock;
    HANDLE wpending;
    HANDLE thread;
    worker *worker;
    void *work[256];
    size_t wcount;
    int closed;
};

void wstart(struct wqueue *q, worker *worker);
void wpush(struct wqueue *q, void *work);
void wclose(struct wqueue *q);
