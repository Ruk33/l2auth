#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <pthread.h>
#endif

struct thread {
	void *buf;

#ifdef _WIN32
	CRITICAL_SECTION lock;
#endif

#ifdef __linux__
	pthread_mutex_t lock;
#endif
};

typedef int thread_cb(struct thread *thread);

void run_in_thread(struct thread *thread, thread_cb *function);

void sleep(int ms);

void lock(struct thread *thread);

void unlock(struct thread *thread);
