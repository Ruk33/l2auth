#ifndef SERVER_TIMER_H
#define SERVER_TIMER_H

/**
 * Start the server timer.
 * It will block the thread calling this function.
 */
void *server_timer_start(void *p);

#endif
