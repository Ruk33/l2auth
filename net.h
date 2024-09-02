enum net_event {
    // new connection
    net_conn,
    // client closed the connection
    net_closed,
    // can read
    net_read,
    // no events after 100 ms of timeout
    net_no_events,
};

typedef void (net_handler)
(int socket, enum net_event event, void *read, unsigned long long len);

// create a new socket for ipv4 assigned to a port.
int net_port(unsigned short port);
// create a new socket using a file (.sock, .socket, etc.)
int net_sock(char *path);
// block and listen for events.
void net_listen(int server, net_handler *handler);
// try to send the full buffer. returns the amount
// of bytes that was able to send. if it writes
// less than the intended amount, you will have to 
// keep trying sending the rest of the data.
unsigned long long net_send(int socket, void *buf, unsigned long long n);
// close the socket.
void net_close(int socket);
