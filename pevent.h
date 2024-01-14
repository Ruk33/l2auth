// platform events

enum pevent {
    pevent_init,
    pevent_before_reload,
    pevent_after_reload,
    pevent_socket_connection,
    pevent_socket_request,
    pevent_socket_disconnected,
};

union ppayload {
    struct pevent_socket { int socket; void *read; size_t len; } pevent_socket;
    // ...
};

typedef int peventf(void **buf, enum pevent event, union ppayload *payload);
