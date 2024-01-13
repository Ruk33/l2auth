enum net_event {
    // new connection
    net_conn,
    // client closed the connection
    net_closed,
    // can read
    net_read,
    // can write
    net_write,
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
// less than the intended amount, wait for net_write
// to keep sending past the bytes already sent.
unsigned long long net_send(int socket, void *buf, unsigned long long n);
// close the socket.
void net_close(int socket);
