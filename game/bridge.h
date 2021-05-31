#ifndef BRIDGE_H
#define BRIDGE_H

/**
 * The bridge layer will be in charge
 * of the comunication between the host
 * and the game server library.
 *
 * At initialization, the bridge will
 * call the game server lib initialization
 * so the server data can be allocated.
 *
 * After that, all the communication will
 * be queued to be handled later by the
 * work queue.
 */
void *bridge_init(void);

#endif
