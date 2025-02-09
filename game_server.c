#include "utils.h"
#include "net.h"
#include "thread.h"

#ifdef LIB

struct connection {
    int socket;

    byte encrypt_key[8];
    byte decrypt_key[8];

    byte to_send[512];
    byte request[512];

    u64 to_send_count;
    u64 sent;
    u64 request_count;
};

struct state {
    struct connection connections[1024];
};

void init(byte **memory)
{
    *memory = get_memory(sizeof(struct state));

    struct state *state = *(struct state **) memory;

    trace("init executed" nl);
}

void push_packet(struct connection *connection, byte *end_of_packet, int encrypt)
{   
    byte *start_of_packet = connection->to_send;

    u16 size = (u16) (end_of_packet - start_of_packet);
    
    if (size > 1)
        size -= 2;
    
    if (encrypt) {
        byte *body = start_of_packet + 2;
        
        u32 temp = 0;
        u32 temp2 = 0;
        for (u16 i = 0; i < size; i++) {
            temp2 = body[i] & 0xff;
            body[i] = (byte) (temp2 ^ connection->encrypt_key[i & 7] ^ temp);
            temp = body[i];
        }
        
        u32 old = ((u32) connection->encrypt_key[0] & 0xff);
        old |= ((u32) connection->encrypt_key[1] << 8 & 0xff00);
        old |= ((u32) connection->encrypt_key[2] << 0x10 & 0xff0000);
        old |= ((u32) connection->encrypt_key[3] << 0x18 & 0xff000000);
        
        old += size;
        
        connection->encrypt_key[0] = (byte) (old & 0xff);
        connection->encrypt_key[1] = (byte) (old >> 0x08 & 0xff);
        connection->encrypt_key[2] = (byte) (old >> 0x10 & 0xff);
        connection->encrypt_key[3] = (byte) (old >> 0x18 & 0xff);
    }

    copy_memory(start_of_packet, &size, sizeof(size));
}

void decrypt(struct connection *conn, byte *request)
{
    check(conn);
    check(request);
    
    u16 size = *(u16 *) request;
    if (size > 1)
        size -= 2;
    
    byte *body = request + 2;
    u32 temp = 0;
    u32 temp2 = 0;
    for (u16 i = 0; i < size; i++) {
        temp2 = body[i];
        body[i] = (byte) (temp2 ^ conn->decrypt_key[i & 7] ^ temp);
        temp = temp2;
    }
    
    u32 old = conn->decrypt_key[0] & 0xff;
    old |= conn->decrypt_key[1] << 8 & 0xff00;
    old |= conn->decrypt_key[2] << 0x10 & 0xff0000;
    old |= conn->decrypt_key[3] << 0x18 & 0xff000000;
    
    old += size;
    
    conn->decrypt_key[0] = (byte) (old & 0xff);
    conn->decrypt_key[1] = (byte) (old >> 0x08 & 0xff);
    conn->decrypt_key[2] = (byte) (old >> 0x10 & 0xff);
    conn->decrypt_key[3] = (byte) (old >> 0x18 & 0xff);
}

byte *new_packet(struct connection *connection)
{
    return connection->to_send + sizeof(u16);
}

struct connection *find_connection(struct state *state, int socket)
{
    /*
     * Find active connection.
     */
    for (u64 i = 0; i < array_length(state->connections); i++) {
        struct connection *connection = state->connections + i;
        if (connection->socket == socket)
            return connection;
    }

    /*
     * Find free connection to be used.
     */
    for (u64 i = 0; i < array_length(state->connections); i++) {
        struct connection *connection = state->connections + i;
        if (!connection->socket)
            return connection;
    }

    return 0;
}

void push_protocol(struct connection *connection)
{
    byte type = 0x00;
    
    byte protocol[] = {
        0x01,
        // crypt key
        0x94,
        0x35,
        0x00,
        0x00,
        0xa1,
        0x6c,
        0x54,
        0x87,
    };

    byte *packet = new_packet(connection);

    packet = append(packet, type);
    packet = append(packet, protocol);

    push_packet(connection, packet, 0);
}

void send_queued_packets(struct connection *connection)
{
    byte *head = connection->to_send + connection->sent;
    u64 to_send = connection->to_send_count - connection->sent;

    if (!to_send)
        return;

    trace("sending %d bytes of data" nl, (u32) to_send);
    connection->sent += net_send(connection->socket, head, to_send);

    if (connection->sent >= connection->to_send_count) {
        connection->sent = 0;
        connection->to_send_count = 0;
    }
}

void try_handling_request(struct connection *connection)
{
    byte *request = connection->request;
    
    u16 size = 0;
    copy_memory(&size, request, sizeof(size));
    
    /*
     * Check for incomplete packet.
     */
    if (size > connection->request_count)
        return;

    trace("here you need to handle a packet of type x" nl);
}

void handle_request(struct thread *thread, byte **memory, int socket, enum net_event event, byte *read, u64 len)
{
    struct state *state = *(struct state **) memory;

    switch (event) {
        case net_conn: {
            struct connection *connection = find_connection(state, socket);

            trace("new client trying to connect to game server." nl);

            if (!connection) {
                trace("i can't accept more connection. i will drop the client." nl);
                net_close(socket);
                return;
            }

            trace("connection accepted, will send the init packet." nl);

            byte key[] = {0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87};
            copy_memory(connection->encrypt_key, key, sizeof(key));
            copy_memory(connection->decrypt_key, key, sizeof(key));

            // push_protocol(connection);
            // send_queued_packets(connection);
        } break;

        case net_closed: {
            struct connection *connection = find_connection(state, socket);

            trace("client closed the connection." nl);

            if (connection)
                reset_memory(connection, sizeof(struct connection));
        } break;

        case net_read:
            struct connection *connection = find_connection(state, socket);

            if (!connection)
                return;

            copy_memory(connection->request + connection->request_count, read, len);

            lock(thread);
            connection->request_count += len;
            unlock(thread);
        break;
    }
}

void update(byte **memory)
{
    struct state *state = *(struct state **) memory;

    for (u64 i = 0; i < array_length(state->connections); i++) {
        struct connection *connection = state->connections + i;

        if (!connection->socket)
            continue;

        // "input"
        try_handling_request(connection);

        // update

        send_queued_packets(connection);
    }
}

#else
/*
 * Main file/executable start.
 */

#include "library.h"

typedef void init_cb(byte **memory);
typedef void update_cb(byte **memory);
typedef void handle_request_cb(struct thread *thread, byte **memory, int socket, enum net_event event, byte *read, u64 len);

static struct library library = {0};
static byte *memory = 0;
static struct thread net_thread = {0};

static init_cb *init_from_library = 0;
static update_cb *update_from_library = 0;
static handle_request_cb *handle_request_from_library = 0;

void net_request_handler(int socket, enum net_event event, void *read, unsigned long long len)
{
    handle_request_from_library(&net_thread, &memory, socket, event, read, len);
}

int net_thread_loop(struct thread *thread)
{
    trace("listening for requests in port 7777" nl);

    int socket = net_port(7777);

    net_listen(socket, net_request_handler);

    return 0;
}

int main()
{
    trace("starting game server" nl);

    load_library(&library, "game_server.dll");

    init_from_library = (init_cb *) load_function(&library, "init");

    update_from_library = (update_cb *) load_function(&library, "update");

    handle_request_from_library = (handle_request_cb *) load_function(&library, "handle_request");

    init_from_library(&memory);

    run_in_thread(&net_thread, net_thread_loop);

    while (1) {
        /*
         * ~60fps
         */
        sleep(16);
        update_from_library(&memory);
    }

    return 0;
}

#endif
