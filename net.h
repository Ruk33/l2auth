enum net_event {
    net_conn,
    net_closed,
    net_read,
};

struct net_socket {
#ifdef _WIN32
    SOCKET handle;
#endif

#ifdef __linux__
    int handle;
#endif
};

typedef void (net_handler)(struct net_socket socket, enum net_event event, void *read, int bytes_read);

/*
 * Create a new socket for ipv4 assigned to a port.
 */
struct net_socket net_port(unsigned short port);

/*
 * Block and listen for events.
 */
void net_block_and_listen(struct net_socket socket, net_handler *handler);

/*
 * Try to send the full buffer. returns the amount
 * of bytes that was able to send. if it writes
 * less than the intended amount, you will have to
 * keep trying sending the rest of the data.
 */
int net_send(struct net_socket socket, void *buf, int bytes_count);

void net_close(struct net_socket socket);
