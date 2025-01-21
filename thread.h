typedef int thread_cb(void *buf);

void run_in_thread(thread_cb *function, void *buf);

void sleep(int ms);
