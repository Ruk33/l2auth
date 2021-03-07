#ifndef TIMER_H
#define TIMER_H

typedef void (* timer_callback)(void);

/**
 * Start the timer that ticks every seconds
 * and executes the callback function.
 * This function will block the thread.
 */
void timer_start(unsigned int seconds, timer_callback callback);

#endif
