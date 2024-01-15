#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <pthread.h>
#endif

struct wqueue;
typedef void (worker)(struct wqueue *q, void *work);

struct wqueue {
    void *p;
    worker *worker;
    void *work[256];
    size_t wcount;
    int closed;

#ifdef _WIN32
    HANDLE lock;
    HANDLE wpending;
    HANDLE thread;
#endif

#ifdef __linux__
    pthread_mutex_t lock;
    pthread_cond_t wpending;
    pthread_t thread;
#endif
};

void wstart(struct wqueue *q, worker *worker);
void wpush(struct wqueue *q, void *work);
void wclose(struct wqueue *q);
